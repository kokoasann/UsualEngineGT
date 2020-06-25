#include "PreCompile.h"
#include "AlphaModel.h"
#include "../RenderState.h"

namespace UsualEngine
{
	void AlphaModelRender::Init()
	{
		DXGI_SAMPLE_DESC samp;
		ZeroMemory(&samp, sizeof(samp));
		samp.Count = 1;
		samp.Quality = 0;
		m_alphaRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_D32_FLOAT, samp);
		m_vsCopy.Load("Assets/shader/copy.fx", "VSMain", Shader::EnType::VS);
		m_psCopy.Load("Assets/shader/copy.fx", "PSMain", Shader::EnType::PS);
	}
	void AlphaModelRender::Render(PostEffect* posteffect)
	{
		auto ge = usualEngine()->GetGraphicsEngine();
		auto& gbuffer = ge->GetPreRender().GetGBuffer();

		gbuffer.SetGBuffer();

		int rtcount = 0;
		RenderTarget* oldtarget[8] = { nullptr };
		ge->OMGetRenderTargets(rtcount, oldtarget);

		auto dc = ge->GetD3DDeviceContext();

		//ID3D11ShaderResourceView* srvlist[] = { ge->GetMainRenderTarget()->GetSRV() };
		dc->PSSetShaderResources(enSkinModelSRVReg_SceneTexture, 1, &posteffect->GetCurrentRenderTarget().GetSRV());
		RenderTarget* rendertarget[] = { &m_alphaRenderTarget };
		ge->OMSetRenderTarget(1, rendertarget);

		float color[4] = { 0.f,0.f,0.f,0.f };
		dc->ClearDepthStencilView(m_alphaRenderTarget.GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
		dc->ClearRenderTargetView(m_alphaRenderTarget.GetRTV(), color);

		ID3D11DepthStencilState* oldDS=0;
		unsigned int oldIND = 0;
		dc->OMGetDepthStencilState(&oldDS, &oldIND);
		dc->OMSetDepthStencilState(DepthStencilState::sceneRender, 0);

		for (auto go : m_renderObject)
		{
			go->Render();
		}
		m_renderObject.clear();

		//Œ³‚É–ß‚·
		
		gbuffer.UnSetGBuffer();

		RenderTarget* rtl[] = { &posteffect->GetCurrentRenderTarget() };
		ge->OMSetRenderTarget(1, rtl);
		
		dc->PSSetShaderResources(0, 1, &m_alphaRenderTarget.GetSRV());

		ID3D11BlendState* blendS = nullptr;
		float factor[4];
		unsigned int mask = 0;
		dc->OMGetBlendState(&blendS, factor, &mask);
		dc->OMSetBlendState(BlendState_Trans(), 0, 0xFFFFFFFF);
		//m_pd3dDeviceContext->OMSetDepthStencilState(DepthStencilState::spriteRender, 0);

		dc->VSSetShader((ID3D11VertexShader*)m_vsCopy.GetBody(), 0, 0);
		dc->PSSetShader((ID3D11PixelShader*)m_psCopy.GetBody(), 0, 0);
		dc->IASetInputLayout(m_vsCopy.GetInputLayout());
		
		posteffect->DrawPrimitive();
		dc->OMSetBlendState(blendS, factor, mask);
		ge->OMSetRenderTarget(rtcount, oldtarget);
		dc->OMSetDepthStencilState(oldDS, oldIND);
	}
}