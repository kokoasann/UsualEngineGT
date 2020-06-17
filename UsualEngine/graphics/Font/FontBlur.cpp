#include "PreCompile.h"
#include "FontBlur.h"


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
		m_gausBlur.Init(FRAME_BUFFER_W, FRAME_BUFFER_H);
	}
	void FontBlur::DrawStart()
	{
		auto gEngine = usualEngine()->GetGraphicsEngine();
		auto devcon = gEngine->GetD3DDeviceContext();

		gEngine->OMGetRenderTargets(m_oldRenderNum, m_oldRenderTarget);

		float clearColor[] = {
			0.0f, 0.0f, 0.0f, 0.0f
		};
		devcon->ClearRenderTargetView(m_renderTarget.GetRTV(), clearColor);
		RenderTarget* rt[] = { &m_renderTarget };
		gEngine->OMSetRenderTarget(1, rt);
	}
	void FontBlur::DrawEnd()
	{
		auto gEngine = usualEngine()->GetGraphicsEngine();

		m_gausBlur.SetDispersion(m_blurParam);
		m_gausBlur.Render(m_renderTarget.GetSRV(), gEngine->GetPostEffect().GetPrimitive());
		
		gEngine->OMSetRenderTarget(m_oldRenderNum, m_oldRenderTarget);
	}
}