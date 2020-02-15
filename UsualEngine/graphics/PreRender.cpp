#include "PreCompile.h"
#include "PreRender.h"


namespace UsualEngine
{
	void PreRender::Init(float h, float w)
	{
		m_gBuffer.Init(h, w);
		m_softShadow.Init();
		m_constBuffer.Create(&m_cbData, sizeof(m_cbData));

		/*DXGI_SAMPLE_DESC msaaD;
		ZeroMemory(&msaaD, sizeof(msaaD));
		msaaD.Count = 1;
		msaaD.Quality = 0;
		m_prePostRenderTarget.Create(w, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_D32_FLOAT, msaaD, NULL, NULL, true);*/
	}
	void PreRender::Render()
	{
		m_gBuffer.Draw();
	}
	void PreRender::PrePostRender()
	{
		//auto ge = usualEngine()->GetGraphicsEngine();
		//int rtcount = 0;
		//RenderTarget* oldtarget[8] = { nullptr };
		//ge->OMGetRenderTargets(rtcount, oldtarget);
		//auto dc = ge->GetD3DDeviceContext();
		//ID3D11ShaderResourceView* srvlist[] = { ge->GetMainRenderTarget()->GetSRV() };
		//dc->PSSetShaderResources(enSkinModelSRVReg_SceneTexture, 1,srvlist);
		//RenderTarget* rendertarget[] = { &m_prePostRenderTarget };
		//ge->OMSetRenderTarget(1, rendertarget);
		//float color[4] = { 0.f,0.f,0.f,0.f };
		//dc->ClearDepthStencilView(m_prePostRenderTarget.GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
		//dc->ClearRenderTargetView(m_prePostRenderTarget.GetRTV(), color);
		//
		//for (auto go : m_prePostRenderObject)
		//{
		//	go->Render();
		//}
		//m_prePostRenderObject.clear();

		////Œ³‚É–ß‚·
		//ge->OMSetRenderTarget(rtcount, oldtarget);
	}
	void PreRender::SendDeferrdConstBuffer()
	{
		auto& cam = usualEngine()->GetMainCamera();
		auto dir = cam.GetTarget() - cam.GetPosition();
		dir.Normalize();
		m_cbData.camDir = dir;
		m_cbData.mViewProj.Mul(cam.GetViewMatrix(), cam.GetProjectionMatrix());
		m_cbData.mViewProj.Inverse(m_cbData.mViewProj);
		auto dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		dc->UpdateSubresource(m_constBuffer.GetBody(), 0, 0, &m_cbData, 0, 0);
		dc->PSSetConstantBuffers(0, 1, &m_constBuffer.GetBody());
	}
}