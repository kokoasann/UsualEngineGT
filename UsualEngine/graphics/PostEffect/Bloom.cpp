#include "PreCompile.h"
#include "Bloom.h"
#include "PostEffect.h"
#include "../RenderState.h"

namespace UsualEngine
{
	void Bloom::Init()
	{
		m_vs.Load("Assets/shader/bloom.fx","VSMain", Shader::EnType::VS);
		m_ps.Load("Assets/shader/bloom.fx","PSMain_SamplingLuminance", Shader::EnType::PS);
		m_psCombine.Load("Assets/shader/bloom.fx","PSMain_Combine", Shader::EnType::PS);
		m_psCopy.Load("Assets/shader/copy.fx", "PSMain", Shader::EnType::PS);
		m_vsCopy.Load("Assets/shader/copy.fx", "VSMain", Shader::EnType::VS);

		int w = FRAME_BUFFER_W;
		int h = FRAME_BUFFER_H;
		for(int i=0;i<BLOOM_NUM_DWNSMP;i++)
		{
			int sft = i;
			m_gausBlur[i].Init(w >> sft, h >> sft);
			m_gausBlur[i].SetDispersion(1.f);
		}
		DXGI_SAMPLE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Count = 1;
		desc.Quality = 0;
		m_combineRT.Create(w >> 2, h >> 2, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, desc);
		m_luminanceRT.Create(w, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, desc);

		
	}
	void Bloom::Render(PostEffect* pe)
	{
		auto gEngine = usualEngine()->GetGraphicsEngine();
		auto devcon = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();

		float clearcol[4] = { 0.f,0.f,0.f,1.f };

		//光度抽出！するよ！
		devcon->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), 0, 0);
		devcon->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), 0, 0);
		ID3D11SamplerState* ssl[] = { SamplerState_Liner() };
		devcon->PSSetSamplers(0, 1, ssl);
		int oldnumRT = 0;
		//RenderTarget* oldrtl[4];
		//gEngine->OMGetRenderTargets(oldnumRT, oldrtl);
		ID3D11ShaderResourceView* srl[5] = { pe->GetCurrentRenderTarget().GetSRV() };
		devcon->PSSetShaderResources(0, 1, &pe->GetCurrentRenderTarget().GetSRV());

		RenderTarget* rtl[5] = {&m_luminanceRT};
		gEngine->OMSetRenderTarget(1, rtl);
		devcon->ClearRenderTargetView(m_luminanceRT.GetRTV(), clearcol);
		devcon->IASetInputLayout(m_vs.GetInputLayout());
		pe->DrawPrimitive();

		//ブラーぞいっ
		int numRT=0;
		gEngine->OMGetRenderTargets(numRT, rtl);
		auto Tex = rtl[0]->GetSRV();
		ID3D11ShaderResourceView* blurList[BLOOM_NUM_DWNSMP] = { 0 };
		for (int i = 0;i<BLOOM_NUM_DWNSMP;i++)
		{
			Tex = m_gausBlur[i].Render(Tex,pe->GetPrimitive());
			blurList[i] = Tex;
		}

		// ｽｰﾊﾟｰﾌｧｲｱｰｺﾝﾎﾞｲｯｯ!!!
		D3D11_VIEWPORT vpl[] = { { 0.f,0.f,m_combineRT.GetWidth(),m_combineRT.GetHeight() } };
		devcon->RSSetViewports(1, vpl);
		devcon->OMSetBlendState(BlendState_Disable(), 0, 0xFFFFFFFF);
		devcon->PSSetShaderResources(0, BLOOM_NUM_DWNSMP, blurList);
		devcon->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), 0, 0);
		devcon->PSSetShader((ID3D11PixelShader*)m_psCombine.GetBody(), 0, 0);
		rtl[0] = &m_combineRT;
		gEngine->OMSetRenderTarget(1, rtl);
		devcon->ClearRenderTargetView(m_combineRT.GetRTV(), clearcol);
		pe->DrawPrimitive();

		//最終合成彼女。
		RenderTarget* RT = &pe->GetCurrentRenderTarget();
		rtl[0] = RT;
		gEngine->OMSetRenderTarget(1,rtl);
		vpl[0] = { 0.f,0.f,(float)RT->GetWidth(),(float)RT->GetHeight() };
		blurList[0] = m_combineRT.GetSRV();
		devcon->PSSetShaderResources(0, 1, blurList);
		devcon->RSSetViewports(1, vpl);
		devcon->OMSetBlendState(BlendState_Add(), 0, 0xFFFFFFFF);
		devcon->VSSetShader((ID3D11VertexShader*)m_vsCopy.GetBody(), 0, 0);
		devcon->PSSetShader((ID3D11PixelShader*)m_psCopy.GetBody(), 0, 0);
		pe->DrawPrimitive();

		//お片付け
		devcon->OMSetBlendState(BlendState_Trans(), 0, 0xFFFFFFFF);			//念のためtransにしとく

		ID3D11ShaderResourceView* clsrv[BLOOM_NUM_DWNSMP] = { 0,0,0,0,0 };
		devcon->PSGetShaderResources(0, BLOOM_NUM_DWNSMP, clsrv);			//SRのアンセット
	}
}