#include "PreCompile.h"
#include "FontBlur.h"
#include "../RenderState.h"

namespace UsualEngine
{
	FontBlur::FontBlur()
	{
	}
	FontBlur::~FontBlur()
	{
		
	}
	void FontBlur::Init()
	{
		DXGI_SAMPLE_DESC desc = {};
		desc.Count = 1;
		desc.Quality = 0;
		m_renderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, desc);
		m_gausBlur.Init(FRAME_BUFFER_W*2, FRAME_BUFFER_H*2);
		m_gausBlur_mid.Init(FRAME_BUFFER_W, FRAME_BUFFER_H);
		m_gausBlur_sml.Init(FRAME_BUFFER_W/2, FRAME_BUFFER_H/2);
		//m_gausBlur_mid.Init(FRAME_BUFFER_W*0.5, FRAME_BUFFER_H * 0.5);

		m_psCopy.Load("Assets/shader/copy.fx", "PSMain", Shader::EnType::PS);
		m_vsCopy.Load("Assets/shader/copy.fx", "VSMain", Shader::EnType::VS);
	}
	void FontBlur::DrawStart(const CVector4& clearColor)
	{
		auto gEngine = usualEngine()->GetGraphicsEngine();
		auto devcon = gEngine->GetD3DDeviceContext();

		gEngine->OMGetRenderTargets(m_oldRenderNum, m_oldRenderTarget);

		/*float fclearColor[] = {
			0.0f, 0.0f, 0.0f, 0.0f
		};*/
		devcon->ClearRenderTargetView(m_renderTarget.GetRTV(), clearColor.v);
		RenderTarget* rt[] = { &m_renderTarget };
		gEngine->OMSetRenderTarget(1, rt);
	}
	void FontBlur::DrawEnd()
	{
		auto gEngine = usualEngine()->GetGraphicsEngine();
		auto devcon = gEngine->GetD3DDeviceContext();

		ID3D11ShaderResourceView* Blur = 0;
		ID3D11ShaderResourceView* Blur2 = 0;
		if (m_blurParam < 16.f)
		{
			m_gausBlur.SetDispersion(m_blurParam);
			Blur = m_gausBlur.Render(m_renderTarget.GetSRV(),m_renderTarget.GetWidth(), m_renderTarget.GetHeight(), gEngine->GetPostEffect().GetPrimitive());
		}
		else if(m_blurParam < 32.f)
		{
			m_gausBlur_mid.SetDispersion(m_blurParam-13.f);
			Blur = m_gausBlur_mid.Render(m_renderTarget.GetSRV(), m_renderTarget.GetWidth(), m_renderTarget.GetHeight(), gEngine->GetPostEffect().GetPrimitive());
		}
		else
		{
			m_gausBlur_sml.SetDispersion(m_blurParam-29.f);
			Blur = m_gausBlur_sml.Render(m_renderTarget.GetSRV(), m_renderTarget.GetWidth(), m_renderTarget.GetHeight(), gEngine->GetPostEffect().GetPrimitive());
		}

		D3D11_VIEWPORT viewPort[] = { { 0.f ,0.f ,FRAME_BUFFER_W,FRAME_BUFFER_H } };
		devcon->RSSetViewports(1, viewPort);

		ID3D11DepthStencilState* oldDS = 0;
		unsigned int oldIND = 0;
		devcon->OMGetDepthStencilState(&oldDS, &oldIND);
		
		RenderTarget* RT = gEngine->GetMainRenderTarget();
		
		RenderTarget* rts[] = { RT };
		//gEngine->OMSetRenderTarget(1, &RT);
		//gEngine->OMSetRenderTarget(m_oldRenderNum, m_oldRenderTarget);
		ID3D11RenderTargetView* rtvs[] = { gEngine->GetBackBuffer() };
		devcon->OMSetRenderTargets(1, rtvs, nullptr);
		
		devcon->OMSetDepthStencilState(DepthStencilState::spriteRender, 0);
		devcon->PSSetShaderResources(0, 1, &Blur);
		devcon->VSSetShader((ID3D11VertexShader*)m_vsCopy.GetBody(), 0, 0);
		devcon->PSSetShader((ID3D11PixelShader*)m_psCopy.GetBody(), 0, 0);
		devcon->IASetInputLayout(m_vsCopy.GetInputLayout());
		gEngine->GetPostEffect().DrawPrimitive();

		//gEngine->OMSetRenderTarget(m_oldRenderNum, m_oldRenderTarget);
		devcon->OMSetDepthStencilState(oldDS, oldIND);
	}
}