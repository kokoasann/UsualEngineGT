#include "PreCompile.h"
#include "GaussianBlur.h"
#include "../RenderState.h"

namespace UsualEngine
{
	GaussianBlur::GaussianBlur()
	{
	}
	GaussianBlur::~GaussianBlur()
	{
		Release();
	}
	void GaussianBlur::Init(float w, float h)
	{
		m_vsXBlur.Load("Assets/shader/GausBlur.fx", "VSMain_X", Shader::EnType::VS);
		m_vsYBlur.Load("Assets/shader/GausBlur.fx", "VSMain_Y", Shader::EnType::VS);
		m_psBlur.Load("Assets/shader/GausBlur.fx", "PSMain", Shader::EnType::PS);

		DXGI_SAMPLE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Count = 1;
		desc.Quality = 0;
		m_renderTargetX.Create(w, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, desc);
		m_renderTargetY.Create(w, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, desc);

		m_cb.Create(&m_bp, sizeof(m_bp));
	}
	void GaussianBlur::Release()
	{
		m_renderTargetX.Release();
		m_renderTargetY.Release();
		m_cb.Release();
		m_vsXBlur.Release();
		m_vsYBlur.Release();
		m_psBlur.Release();
	}

	void GaussianBlur::UpdateWeight()
	{
		if (!m_isChangeBOKE)
			return;
		float total = 0;
		for (int i = 0; i < BLUR_NUM_WEIGHT; i++) {
			m_bp.weights[i] = expf(-0.5f * (float)(i * i) / m_dispersion);
			total += 2.0f * m_bp.weights[i];
		}
		// ‹KŠi‰»
		for (int i = 0; i < BLUR_NUM_WEIGHT; i++) {
			m_bp.weights[i] /= total;
		}
		m_isChangeBOKE = false;
	}

	ID3D11ShaderResourceView* GaussianBlur::Render(ID3D11ShaderResourceView* src,Primitive* primitive)
	{
		auto gEngine = usualEngine()->GetGraphicsEngine();
		auto devcon = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		auto dev = usualEngine()->GetGraphicsEngine()->GetD3DDevice();

		UpdateWeight();
		float clearColor[] = {
			0.0f, 0.0f, 0.0f, 0.0f
		};
		
		RenderTarget** oldRT = 0;
		int oldNum = 0;
		gEngine->OMGetRenderTargets(oldNum, oldRT);

		ID3D11SamplerState* ssl[] = { SamplerState_Liner() };
		devcon->PSSetSamplers(0, 1, ssl);

		RenderTarget* rtl[] = { &m_renderTargetX };
		gEngine->OMSetRenderTarget(1, rtl);
		m_bp.offset.x = 16.f / m_renderTargetX.GetWidth();
		m_bp.offset.y = 0.f;
		devcon->UpdateSubresource(m_cb.GetBody(), 0,0,&m_bp,0,0);
		devcon->ClearRenderTargetView(m_renderTargetX.GetRTV(), clearColor);
		devcon->VSSetShaderResources(0, 1, &src);
		devcon->PSSetShaderResources(0, 1, &src);
		devcon->PSSetConstantBuffers(0, 1, &m_cb.GetBody());
		devcon->VSSetShader((ID3D11VertexShader*)m_vsXBlur.GetBody(),0,0);
		devcon->PSSetShader((ID3D11PixelShader*)m_psBlur.GetBody(), 0, 0);
		primitive->Draw();

		rtl[0] = &m_renderTargetY;
		gEngine->OMSetRenderTarget(1, rtl);
		m_bp.offset.x = 0.f;
		m_bp.offset.y = 16.f / m_renderTargetY.GetWidth();
		devcon->UpdateSubresource(m_cb.GetBody(), 0, 0, &m_bp, 0, 0);
		devcon->ClearRenderTargetView(m_renderTargetY.GetRTV(), clearColor);
		devcon->VSSetShaderResources(0, 1, &m_renderTargetX.GetSRV());
		devcon->PSSetShaderResources(0, 1, &m_renderTargetX.GetSRV());
		devcon->PSSetConstantBuffers(0, 1, &m_cb.GetBody());
		devcon->VSSetShader((ID3D11VertexShader*)m_vsYBlur.GetBody(), 0, 0);
		devcon->PSSetShader((ID3D11PixelShader*)m_psBlur.GetBody(), 0, 0);
		primitive->Draw();

		return m_renderTargetY.GetSRV();
	}
}