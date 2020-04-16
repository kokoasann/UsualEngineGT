#include "PreCompile.h"
#include "GBuffer.h"

#include "GPUViewer.h"

namespace UsualEngine
{
	void GBuffer::Init(float fh, float fw)
	{
		DXGI_SAMPLE_DESC msaaD;
		ZeroMemory(&msaaD, sizeof(msaaD));
		msaaD.Count = 1;
		msaaD.Quality = 0;
		int h = (int)fh, w = (int)fw;
		bool res = false;
		res = m_gBuffer[GB_Diffuse].Create(w, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_D32_FLOAT, msaaD);
		res = m_gBuffer[GB_Specular].Create(w, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, msaaD);
		res = m_gBuffer[GB_Depth].Create(w, h, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_UNKNOWN, msaaD);
		res = m_gBuffer[GB_Normal].Create(w, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, msaaD);
		res = m_gBuffer[GB_Shadow].Create(w, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, msaaD);
		res = m_gBuffer[GB_Tangent].Create(w, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, msaaD);
	}
	void GBuffer::SetGBuffer()
	{
		auto dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		ID3D11ShaderResourceView* list[GB_Num] =
		{
			m_gBuffer[GB_Diffuse].GetSRV(),
			m_gBuffer[GB_Specular].GetSRV(),
			m_gBuffer[GB_Depth].GetSRV(),
			m_gBuffer[GB_Normal].GetSRV(),
			m_gBuffer[GB_Shadow].GetSRV(),
			m_gBuffer[GB_Tangent].GetSRV()
		};
		dc->PSSetShaderResources(enSkinModelSRVReg_GDiffuseMap, GB_Num, list);
		/*for (int i = 0; i < GB_Num; i++)
		{
			dc->PSSetShaderResources(enSkinModelSRVReg_GDiffuseMap + i, 1, &m_gBuffer[i].GetSRV());
		}*/
	}
	void GBuffer::UnSetGBuffer()
	{
		auto dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		ID3D11ShaderResourceView* list[GB_Num] =
		{
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
		};
		dc->PSSetShaderResources(enSkinModelSRVReg_GDiffuseMap, GB_Num, list);
	}
	void GBuffer::Draw()
	{
		auto ge = usualEngine()->GetGraphicsEngine();
		auto dc = ge->GetD3DDeviceContext();
		auto& cam = usualEngine()->GetMainCamera();
		//クリアカラー
		float cc[GB_Num][4] =
		{
			{0.5f, 0.5f, 0.5f, 1.f},
			{0.f, 0.f, 0.f, 1.f},
			{1.f, cam.GetFar()-cam.GetNear(), 1.f, 1.f},
			{0.f, 1.f, 0.f, 1.f},
			{0.f, 0.f, 0.f, 1.f},
			{1.f, 0.f, 0.f, 1.f}
		};
		for (auto i = 0; i < GB_Num; i++)
		{
			dc->ClearRenderTargetView(m_gBuffer[i].GetRTV(), cc[i]);
		}
		dc->ClearDepthStencilView(m_gBuffer[0].GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
		int oldRTcount = 0;
		RenderTarget* oldRT[7] = {NULL};
		ge->OMGetRenderTargets(oldRTcount, oldRT);

		RenderTarget* RTlist[GB_Num] = { 0 };
		for (int i = 0; i < GB_Num; i++)
		{
			RTlist[i] = &m_gBuffer[i];
		}
		ge->OMSetRenderTarget(GB_Num, RTlist);

		for (auto go : m_modelList)
		{
			go->Render();
		}
		m_modelList.clear();

		//コリジョンの描画。
		if (Debug::Instance().debugState.isPhysicsDebugDraw)
		{
			Physics().DebugDraw();
		}

		ge->OMSetRenderTarget(oldRTcount, oldRT);
	}
}