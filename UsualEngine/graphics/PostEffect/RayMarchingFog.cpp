#include "PreCompile.h"
#include "RayMarchingFog.h"

#include "../ShaderSample.h"
#include "../RenderState.h"

namespace UsualEngine
{
	RayMarchingFog::RayMarchingFog()
	{
	}
	RayMarchingFog::~RayMarchingFog()
	{
		Release();
	}
	void RayMarchingFog::Init()
	{
		m_cBuffer.Create(nullptr, sizeof(m_cbData));
		m_ps.Load("Assets/Shader/RayMarchingFog.fx", "PSMain_RMFog", Shader::EnType::PS);

		DXGI_SAMPLE_DESC samp;
		ZeroMemory(&samp, sizeof(samp));
		samp.Count = 1;
		samp.Quality = 0;
		m_rtFog.Create(FRAME_BUFFER_W * 0.5f, FRAME_BUFFER_H * 0.5f, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, samp);
		m_rtVolume.Create(FRAME_BUFFER_W * 0.5f, FRAME_BUFFER_H * 0.5f, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, samp);
		m_blur.Init(FRAME_BUFFER_W, FRAME_BUFFER_H);
		m_blur.SetDispersion(0.5f);
	}
	void RayMarchingFog::Release()
	{
		m_cBuffer.Release();
		m_ps.Release();
	}
	void RayMarchingFog::Render(PostEffect* pe)
	{
		auto ge = usualEngine()->GetGraphicsEngine();
		auto dc = ge->GetD3DDeviceContext();
		auto& cam = usualEngine()->GetMainCamera();
		auto& shadowmap = ge->GetShadowMap();
		auto lig = ge->GetLightManager().GetMainLightDirection();

		shadowmap.Send2GPU();
		if (lig != nullptr)
		{
			m_cbData.mainLightDir = lig->GetDir();
			m_cbData.mainLightColor = lig->GetCol();
		}

		m_cbData.camPos = cam.GetPosition();
		m_cbData.mVP.Mul(cam.GetViewMatrix(), cam.GetProjectionMatrix());
		m_cbData.mVPI.Inverse(m_cbData.mVP);

		m_cbData.offset += m_offsetSpeed * gameTime()->GetDeltaTime();
		if (m_cbData.offset >m_maxOffset || m_cbData.offset < 0.f)
		{
			m_offsetSpeed *= -1.f;
		}
		m_cbData.camFar = 5000;
		m_cbData.camNear = cam.GetNear();

		ID3D11BlendState* oldbs[1];
		float oldbf[4] = { 0 };
		unsigned int oldbm = 0;
		dc->OMGetBlendState(oldbs, oldbf, &oldbm);
		dc->OMSetBlendState(BlendState::trans, 0, 0xFFFFFFFF);

		ID3D11DepthStencilState* oldDS = 0;
		unsigned int oldIND = 0;
		dc->OMGetDepthStencilState(&oldDS, &oldIND);
		dc->OMSetDepthStencilState(DepthStencilState::spriteRender, 0);

		float ccolor_fog[] = { 1,1,1,0 };
		float ccolor_vol[] = { 0,0,0,0 };
		dc->ClearRenderTargetView(m_rtFog.GetRTV(), ccolor_fog);
		dc->ClearRenderTargetView(m_rtVolume.GetRTV(), ccolor_vol);

		RenderTarget* rts[] = { &m_rtFog,&m_rtVolume };
		ge->OMSetRenderTarget(2, rts);
		D3D11_VIEWPORT vp[] = { { 0.f,0.f,(FLOAT)rts[0]->GetWidth(),(FLOAT)rts[0]->GetHeight() } ,
								{ 0.f,0.f,(FLOAT)rts[1]->GetWidth(),(FLOAT)rts[1]->GetHeight() } };

		dc->RSSetViewports(2, vp);

		dc->UpdateSubresource(m_cBuffer.GetBody(), 0, 0, &m_cbData, 0, 0);
		dc->PSSetConstantBuffers(enSkinModelCBReg_Generic, 1, &m_cBuffer.GetBody());
		
		dc->VSSetShader((ID3D11VertexShader*)ShaderSample::VS_Copy.GetBody(), 0,0);
		dc->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), 0, 0);
		dc->IASetInputLayout(ShaderSample::VS_Copy.GetInputLayout());

		pe->DrawPrimitive();

		//blur
		auto blured = m_blur.Render(m_rtFog.GetSRV(), FRAME_BUFFER_W * 0.5f, FRAME_BUFFER_H * 0.5f, pe->GetPrimitive());


		//copy

		rts[0] = { &pe->GetCurrentRenderTarget() };
		ge->OMSetRenderTarget(1, rts);
		vp[0] = { 0.f,0.f,(FLOAT)rts[0]->GetWidth(),(FLOAT)rts[0]->GetHeight() };
		dc->RSSetViewports(1, vp);

		dc->PSSetShaderResources(0, 1, &blured);

		dc->VSSetShader((ID3D11VertexShader*)ShaderSample::VS_Copy.GetBody(), NULL, 0);
		dc->PSSetShader((ID3D11PixelShader*)ShaderSample::PS_Copy.GetBody(), NULL, 0);
		dc->IASetInputLayout(ShaderSample::VS_Copy.GetInputLayout());

		pe->DrawPrimitive();


		dc->OMSetBlendState(BlendState::add, 0, 0xFFFFFFFF);
		dc->PSSetShaderResources(0, 1, &m_rtVolume.GetSRV());

		dc->VSSetShader((ID3D11VertexShader*)ShaderSample::VS_Copy.GetBody(), NULL, 0);
		dc->PSSetShader((ID3D11PixelShader*)ShaderSample::PS_Copy.GetBody(), NULL, 0);
		dc->IASetInputLayout(ShaderSample::VS_Copy.GetInputLayout());

		//pe->DrawPrimitive();

		dc->OMSetDepthStencilState(oldDS, oldIND);
		dc->OMSetBlendState(oldbs[0], oldbf, oldbm);
	}
}