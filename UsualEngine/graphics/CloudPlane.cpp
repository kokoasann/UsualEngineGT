#include "PreCompile.h"
#include "CloudPlane.h"
#include "RenderState.h"
#include "ShaderSample.h"

namespace UsualEngine
{
	struct SimpleVertex
	{
		CVector4 pos;
		CVector3 Normal;
		CVector3 Tangent;
		CVector2 uv;
		
	};
	CloudPlane::CloudPlane()
	{
	}
	CloudPlane::~CloudPlane()
	{
		Release();
	}

	void CloudPlane::Init()
	{
		m_cBuffer.Create(nullptr, sizeof(m_cbData));
		m_vscBuffer.Create(nullptr, sizeof(SkinModel::SVSConstantBuffer));

		m_vs.Load("Assets/shader/model.fx", "VSMain", Shader::EnType::VS);
		m_ps.Load("Assets/shader/Cloud.fx", "PSMain_PlaneCloud", Shader::EnType::PS);

		DXGI_SAMPLE_DESC samp;
		ZeroMemory(&samp, sizeof(samp));
		samp.Count = 1;
		samp.Quality = 0;
		m_rt.Create(FRAME_BUFFER_W*0.5f, FRAME_BUFFER_H * 0.5f, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, samp);
		

		CVector2 hsize = {0.5f,0.5f};
		SimpleVertex topo[] =
		{
			{
				CVector4(-hsize.x, 0.0f, hsize.y, 1.0f),
				CVector3(0.f,-1.f,0.f),
				CVector3(1.f,0.f,0.f),
				CVector2(0.0f,1.0f)
			},
			{
				CVector4(hsize.x, 0.0f, hsize.y, 1.0f),
				CVector3(0.f,-1.f,0.f),
				CVector3(1.f,0.f,0.f),
				CVector2(1.0f, 1.0f)
			},
			{
				CVector4(-hsize.x, 0.0f, -hsize.y, 1.0f),
				CVector3(0.f,-1.f,0.f),
				CVector3(1.f,0.f,0.f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(hsize.x, 0.0f, -hsize.y, 1.0f),
				CVector3(0.f,-1.f,0.f),
				CVector3(1.f,0.f,0.f),
				CVector2(1.0f, 0.0f)
			}
		};
		short index[] = { 0,1,2,3 };
		m_prim.Cteate(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, sizeof(SimpleVertex), topo, 4, IndexBuffer::e16bit, index);
	}
	void CloudPlane::Release()
	{
		m_cBuffer.Release();
		m_vs.Release();
		m_ps.Release();
		m_prim.Release();
	}


	void CloudPlane::Render(PostEffect* pe)
	{
		auto ge = usualEngine()->GetGraphicsEngine();
		auto dc = ge->GetD3DDeviceContext();
		auto& cam = usualEngine()->GetMainCamera();
		auto& gbuffer = ge->GetPreRender().GetGBuffer();
		
		auto gDepth = gbuffer.GetGBuffer(GBuffer::GB_Depth);
		dc->PSSetShaderResources(enSkinModelSRVReg_GDepthMap, 1, &gDepth->GetSRV());
		
		ID3D11DepthStencilState* oldDS = 0;
		unsigned int oldIND = 0;
		dc->OMGetDepthStencilState(&oldDS, &oldIND);
		dc->OMSetDepthStencilState(DepthStencilState::spriteRender, 0);

		ID3D11BlendState* oldbs[1];
		float oldbf[4] = { 0 };
		unsigned int oldbm = 0;
		dc->OMGetBlendState(oldbs, oldbf, &oldbm);
		dc->OMSetBlendState(BlendState::trans, 0, 0xFFFFFFFF);

		float clearCol[4] = { 1,1,1,0 };
		dc->ClearRenderTargetView(m_rt.GetRTV(), clearCol);
		
		D3D11_VIEWPORT vp[] = { { 0.f,0.f,m_rt.GetWidth(),m_rt.GetHeight() } };
		dc->RSSetViewports(1, vp);
		RenderTarget* rts[] = { &m_rt};
		ge->OMSetRenderTarget(1, rts);
		

		m_cbData.campos = cam.GetPosition();
		auto mainLight = ge->GetLightManager().GetMainLightDirection();
		if(mainLight != nullptr)
			m_cbData.mainLightDir = mainLight->GetDir();
		dc->UpdateSubresource(m_cBuffer.GetBody(), 0, 0, &m_cbData, 0, 0);
		dc->PSSetConstantBuffers(enSkinModelCBReg_Generic, 1, &m_cBuffer.GetBody());

		SkinModel::SVSConstantBuffer vsbuf;
		vsbuf.camDir = CVector3::Zero();
		vsbuf.mProj = cam.GetProjectionMatrix();
		vsbuf.mView = cam.GetViewMatrix();
		
		CMatrix sca, rot, pos,world;
		sca.MakeScaling(m_scale);
		rot.MakeRotationFromQuaternion(m_rot);
		pos.MakeTranslation(m_pos);
		world.Mul(sca, rot);
		world.Mul(world, pos);

		vsbuf.mWorld = world;
		dc->UpdateSubresource(m_vscBuffer.GetBody(), 0, 0, &vsbuf, 0, 0);
		dc->VSSetConstantBuffers(enSkinModelCBReg_VSPS, 1, &m_vscBuffer.GetBody());

		dc->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), NULL, 0);
		dc->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), NULL, 0);
		dc->IASetInputLayout(m_vs.GetInputLayout());

		m_prim.Draw();

		//copy
		rts[0] = { &pe->GetCurrentRenderTarget() };
		ge->OMSetRenderTarget(1, rts);
		vp[0] = { 0.f,0.f,(FLOAT)rts[0]->GetWidth(),(FLOAT)rts[0]->GetHeight() };
		dc->RSSetViewports(1, vp);

		dc->PSSetShaderResources(0, 1, &m_rt.GetSRV());

		dc->VSSetShader((ID3D11VertexShader*)ShaderSample::VS_Copy.GetBody(), NULL, 0);
		dc->PSSetShader((ID3D11PixelShader*)ShaderSample::PS_Copy.GetBody(), NULL, 0);
		dc->IASetInputLayout(ShaderSample::VS_Copy.GetInputLayout());

		pe->DrawPrimitive();

		//Œãˆ—B
		dc->OMSetDepthStencilState(oldDS, oldIND);
		dc->OMSetBlendState(oldbs[0], oldbf, oldbm);
		
	}
}