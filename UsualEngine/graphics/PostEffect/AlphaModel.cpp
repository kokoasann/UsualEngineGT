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
		m_psTripleCopy.Load("Assets/shader/copy.fx", "PSMain_Triple", Shader::EnType::PS);

		bool res;
		res = m_depthRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_UNKNOWN, samp);
		res = m_normalRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, samp);
		res = m_specularRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, samp);
	}
	void AlphaModelRender::Render(PostEffect* posteffect)
	{
		auto ge = usualEngine()->GetGraphicsEngine();
		auto& gbuffer = ge->GetPreRender().GetGBuffer();

		int rtcount = 0;
		RenderTarget* oldtarget[8] = { nullptr };
		ge->OMGetRenderTargets(rtcount, oldtarget);

		auto dc = ge->GetD3DDeviceContext();

		//ID3D11ShaderResourceView* srvlist[] = { ge->GetMainRenderTarget()->GetSRV() };
		
		//##################################################################
		//GBufferの法線、深度値、スペキュラに書き込むためこっちで用意したバッファにコピーする
		//##################################################################

		float color[4] = { 0.f,0.f,0.f,0.f };
		dc->ClearDepthStencilView(m_alphaRenderTarget.GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
		dc->ClearRenderTargetView(m_alphaRenderTarget.GetRTV(), color);
		dc->ClearRenderTargetView(m_depthRenderTarget.GetRTV(), color);
		dc->ClearRenderTargetView(m_normalRenderTarget.GetRTV(), color);
		dc->ClearRenderTargetView(m_specularRenderTarget.GetRTV(), color);

		//必要なGBufferのポインタをゲット
		RenderTarget* gdepth = gbuffer.GetGBuffer(GBuffer::GB_Depth);
		RenderTarget* gnormal = gbuffer.GetGBuffer(GBuffer::GB_Normal);
		RenderTarget* gspecular = gbuffer.GetGBuffer(GBuffer::GB_Specular);
		{
			//ここでコピーの工程。
			dc->VSSetShader((ID3D11VertexShader*)m_vsCopy.GetBody(), 0, 0);
			dc->PSSetShader((ID3D11PixelShader*)m_psTripleCopy.GetBody(), 0, 0);
			dc->IASetInputLayout(m_vsCopy.GetInputLayout());

			RenderTarget* rts[] = 
			{ 
				&m_depthRenderTarget,
				&m_normalRenderTarget,
				&m_specularRenderTarget,
			};
			ge->OMSetRenderTarget(3, rts);

			ID3D11ShaderResourceView* list[] =
			{
				gdepth->GetSRV(),
				gnormal->GetSRV(),
				gspecular->GetSRV(),
			};
			dc->PSSetShaderResources(0, 3, list);

			posteffect->DrawPrimitive();

			//後処理のため、nullを入れる。
			list[0] = nullptr;
			list[1] = nullptr;
			list[2] = nullptr;
			dc->PSSetShaderResources(0, 3, list);
		}


		//#######################
		//こっからアルファモデルの描画。
		//#######################

		ID3D11DepthStencilState* oldDS=0;
		unsigned int oldIND = 0;
		dc->OMGetDepthStencilState(&oldDS, &oldIND);
		dc->OMSetDepthStencilState(DepthStencilState::sceneRender, 0);

		//書き込むためのGBufferをセット。
		RenderTarget* rendertarget[] = { &m_alphaRenderTarget ,gdepth ,gnormal ,gspecular};
		ge->OMSetRenderTarget(4, rendertarget);

		gbuffer.SetGBuffer();
		//さっきコピーした読み込み用のものを入れる
		dc->PSSetShaderResources(enSkinModelSRVReg_GDepthMap, 1, &m_depthRenderTarget.GetSRV());
		dc->PSSetShaderResources(enSkinModelSRVReg_GNormalMap, 1, &m_normalRenderTarget.GetSRV());
		dc->PSSetShaderResources(enSkinModelSRVReg_GSpecularMap, 1, &m_specularRenderTarget.GetSRV());

		dc->PSSetShaderResources(enSkinModelSRVReg_SceneTexture, 1, &posteffect->GetCurrentRenderTarget().GetSRV());
		//レンダー！
		for (auto go : m_renderObject)
		{
			go->Render();
		}
		m_renderObject.clear();

		//元に戻す
		gbuffer.UnSetGBuffer();

		//カレントRTに合成。
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

		//後処理。
		dc->OMSetBlendState(blendS, factor, mask);
		ge->OMSetRenderTarget(rtcount, oldtarget);
		dc->OMSetDepthStencilState(oldDS, oldIND);
	}
}