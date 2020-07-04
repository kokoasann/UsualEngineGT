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
	void GaussianBlur::Init(int w, int h)
	{
		{
			auto dev = usualEngine()->GetGraphicsEngine()->GetD3DDevice();
			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			//desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			//desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			//desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			dev->CreateBlendState(&desc, &m_blendstate);
		}

		m_vsXBlur.Load("Assets/shader/GausBlur.fx", "VSMain_X", Shader::EnType::VS);
		m_vsYBlur.Load("Assets/shader/GausBlur.fx", "VSMain_Y", Shader::EnType::VS);
		m_psBlur.Load("Assets/shader/GausBlur.fx", "PSMain", Shader::EnType::PS);

		DXGI_SAMPLE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Count = 1;
		desc.Quality = 0;
		m_renderTargetX.Create(w>>1, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, desc);
		m_renderTargetY.Create(w>>1, h>>1, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, desc);

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
		m_blendstate->Release();
	}

	void GaussianBlur::UpdateWeight()
	{
		if (!m_isChangeBOKE)
			return;

		if (m_dispersion == 0.f)
		{
			for (int i = 0; i < BLUR_NUM_WEIGHT; i++)
			{
				m_bp.weights[i] = 0.f;
			}
			return;
		}

		float total = 0;
		for (int i = 0; i < BLUR_NUM_WEIGHT; i++) {
			m_bp.weights[i] = expf(-0.5f * (float)(i * i) / m_dispersion);
			total += 2.0f * m_bp.weights[i];
		}
		// 規格化
		for (int i = 0; i < BLUR_NUM_WEIGHT; i++) {
			m_bp.weights[i] /= total;
		}
		m_isChangeBOKE = false;
	}

	ID3D11ShaderResourceView* GaussianBlur::Render(ID3D11ShaderResourceView* src, float w, float h,Primitive* primitive)
	{
		auto gEngine = usualEngine()->GetGraphicsEngine();
		auto devcon = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();

		UpdateWeight();
		float clearColor[] = {
			0.0f, 0.0f, 0.0f, 0.0f
		};
		ID3D11BlendState* oldblend = 0;
		float oldBF[4] = { 0 };
		UINT oldSM = 0;
		devcon->OMGetBlendState(&oldblend, oldBF, &oldSM);

		devcon->OMSetBlendState(m_blendstate, 0, 0xffffffff);

		
		RenderTarget* oldRT[7] = {0};
		int oldNum = 0;
		gEngine->OMGetRenderTargets(oldNum, oldRT);
		D3D11_VIEWPORT oldvpl[4]={0};
		//D3D11_VIEWPORT* oldvpl=0;
		unsigned int oldnumvpl = 0;
		devcon->RSGetViewports(&oldnumvpl, oldvpl);

		ID3D11SamplerState* ssl[] = { SamplerState_Liner() };
		devcon->PSSetSamplers(0, 1, ssl);


		RenderTarget* rtl[] = { &m_renderTargetX };
		gEngine->OMSetRenderTarget(1, rtl);

		for (int i = 0; i < BLUR_NUM_WEIGHT; i++) {
			//m_bp.weights[i] = 0.f;
		}
		//m_bp.weights[7] = 0.5f;
		//m_bp.weights[0] = 0.3f;

		//フィルターのサイズが16*16なので8ピクセル分動かす
		//m_bp.offset.x = 8.f / (m_renderTargetX.GetWidth()<<1);	//本来ここは引数で送られて来たテクスチャのサイズを使う。
		m_bp.offset.x = 32.f / w;	//テクスチャの縮小の関係でYブラーの4倍オフセットをする必要がある。
		m_bp.offset.y = 0.f;

		devcon->UpdateSubresource(m_cb.GetBody(), 0,0,&m_bp,0,0);
		devcon->ClearRenderTargetView(m_renderTargetX.GetRTV(), clearColor);
		devcon->VSSetShaderResources(0, 1, &src);
		devcon->PSSetShaderResources(0, 1, &src);
		devcon->PSSetConstantBuffers(0, 1, &m_cb.GetBody());
		devcon->VSSetShader((ID3D11VertexShader*)m_vsXBlur.GetBody(),0,0);
		devcon->PSSetShader((ID3D11PixelShader*)m_psBlur.GetBody(), 0, 0);
		devcon->IASetInputLayout(m_vsXBlur.GetInputLayout());
		D3D11_VIEWPORT vpl[] = { { 0.f, 0.f, m_renderTargetX.GetWidth(), m_renderTargetX.GetHeight() } };
		devcon->RSSetViewports(1, vpl);
		primitive->Draw();


		rtl[0] = &m_renderTargetY;
		gEngine->OMSetRenderTarget(1, rtl);
		m_bp.offset.x = 0.f;
		m_bp.offset.y = 8.f / m_renderTargetX.GetHeight();
		//m_bp.offset.y = 0;

		devcon->UpdateSubresource(m_cb.GetBody(), 0, 0, &m_bp, 0, 0);
		devcon->ClearRenderTargetView(m_renderTargetY.GetRTV(), clearColor);
		devcon->VSSetShaderResources(0, 1, &m_renderTargetX.GetSRV());
		devcon->PSSetShaderResources(0, 1, &m_renderTargetX.GetSRV());
		devcon->PSSetConstantBuffers(0, 1, &m_cb.GetBody());
		devcon->VSSetShader((ID3D11VertexShader*)m_vsYBlur.GetBody(), 0, 0);
		devcon->PSSetShader((ID3D11PixelShader*)m_psBlur.GetBody(), 0, 0);
		devcon->IASetInputLayout(m_vsYBlur.GetInputLayout());
		vpl[0] = { 0.f, 0.f, (float)m_renderTargetY.GetWidth(), (float)m_renderTargetY.GetHeight() };
		devcon->RSSetViewports(1, vpl);
		primitive->Draw();

		gEngine->OMSetRenderTarget(oldNum, oldRT);
		devcon->OMSetBlendState(oldblend, oldBF, oldSM);
		//devcon->RSSetViewports(oldnumvpl, oldvpl);

		return m_renderTargetY.GetSRV();
	}
}