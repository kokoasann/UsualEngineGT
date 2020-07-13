#include "PreCompile.h"
#include "SSReflection.h"
#include "PostEffect.h"

namespace UsualEngine
{
	SSReflection::SSReflection()
	{
	}
	SSReflection::~SSReflection()
	{
	}
	void SSReflection::Init()
	{
		m_vs.Load("Assets/shader/Copy.fx", "VSMain", Shader::EnType::VS);
		m_ps.Load("Assets/shader/SSR.fx", "PSMain_SSR",Shader::EnType::PS);
		m_copyPS.Load("Assets/shader/Copy.fx", "PSMain", Shader::EnType::PS);

		m_constBuf.Create(&m_buf, sizeof(m_buf));

		DXGI_SAMPLE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Count = 1;
		desc.Quality = 0;
		m_rtBuffer.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, 
			DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, desc);
	}
	void SSReflection::Release()
	{
	}
	void SSReflection::Render(PostEffect* pe)
	{
		auto ge = usualEngine()->GetGraphicsEngine();
		auto dc = ge->GetD3DDeviceContext();

		float ccolor[] = { 0.f,0.f,0.f,0.f };
		dc->ClearRenderTargetView(m_rtBuffer.GetRTV(), ccolor);

		auto& rt = pe->GetCurrentRenderTarget();

		RenderTarget* rts[] = { &m_rtBuffer };
		ge->OMSetRenderTarget(1, rts);

		dc->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), 0, 0);
		dc->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), 0, 0);
		dc->IASetInputLayout(m_vs.GetInputLayout());

		ge->GetPreRender().GetGBuffer().SetGBuffer();
		dc->PSSetShaderResources(enSkinModelSRVReg_Textur_1, 1, &rt.GetSRV());

		auto& cam = usualEngine()->GetMainCamera();
		//m_buf.projMat = cam.GetProjectionMatrix();
		//m_buf.invProjMat.Inverse(m_buf.projMat);
		//m_buf.viewMat = cam.GetViewMatrix();
		m_buf.VPMat.Mul(cam.GetViewMatrix(), cam.GetProjectionMatrix());
		m_buf.invVPMat.Inverse(m_buf.VPMat);
		m_buf.campos = cam.GetPosition();
		m_buf.camFar = cam.GetFar();
		m_buf.camNear = cam.GetNear();
		m_buf.rayLen = 1024.f;
		m_buf.rayStep = 1.f;

		dc->UpdateSubresource(m_constBuf.GetBody(), 0, 0, &m_buf, 0, 0);
		dc->PSSetConstantBuffers(7, 1, &m_constBuf.GetBody());
		
		pe->DrawPrimitive();
		
		//dc->PSSetConstantBuffers(7, 1, nullptr);
		ge->GetPreRender().GetGBuffer().UnSetGBuffer();


		//ƒJƒŒƒ“ƒgRT‚É‡¬B
		rts[0] = &rt;
		ge->OMSetRenderTarget(1, rts);

		dc->PSSetShader((ID3D11PixelShader*)m_copyPS.GetBody(), 0, 0);
		dc->PSSetShaderResources(0, 1, &m_rtBuffer.GetSRV());

		pe->DrawPrimitive();
	}
}