#include "PreCompile.h"
#include "PostEffect.h"
#include "graphics/RenderState.h"

namespace UsualEngine
{
	struct SSimpleVertex {
		CVector4 pos;
		CVector2 tex;
	};

	PostEffect::PostEffect()
	{
	}
	PostEffect::~PostEffect()
	{
	}
	void PostEffect::Init()
	{
		InitRenderTarget();
		InitPrimitive();
		m_bloom.Init();
		m_alphaModelRender.Init();
		m_ssr.Init();

		m_cloudPlane.Init();
		m_rmFog.Init();
	}
	void PostEffect::InitRenderTarget()
	{
		DXGI_SAMPLE_DESC samp;
		ZeroMemory(&samp, sizeof(samp));
		samp.Count = 1;
		samp.Quality = 0;
		m_renderTarget[0].Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, samp);
		m_renderTarget[1].Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, samp);
	}
	void PostEffect::InitPrimitive()
	{
		SSimpleVertex vertices[] =
		{
			{
				CVector4(-1.0f, -1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 1.0f),
			},
			{
				CVector4(1.0f, -1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 1.0f),
			},
			{
				CVector4(-1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 0.0f)
			}
		};
		short ind[] = { 0, 1, 2, 3 };
		m_primitive.Cteate(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, sizeof(SSimpleVertex), vertices, 4, IndexBuffer::e16bit,ind);
	}

	void PostEffect::Render()
	{
		auto gEngine = usualEngine()->GetGraphicsEngine();
		auto devcon = gEngine->GetD3DDeviceContext();

		float ccol[] = { 0,0,0,0 };
		devcon->ClearRenderTargetView(GetCurrentRenderTarget().GetRTV(), ccol);

		devcon->ResolveSubresource(
			GetCurrentRenderTarget().GetRenderTarget(),
			0,
			gEngine->GetMainRenderTarget()->GetRenderTarget(),
			0, 
			*GetCurrentRenderTarget().GetTexFormat()
		);

		//gEngine->GetLightManager().RenderPointLight();
		
		
		m_cloudPlane.Render(this);

		static bool isAlphaRender = true;
		if (isAlphaRender)
			m_alphaModelRender.Render(this);

		m_rmFog.Render(this);
		
		m_ssr.Render(this);
		m_bloom.Render(this);
		
		
		devcon->OMSetBlendState(BlendState_Trans(), 0, 0xFFFFFFFF);			//”O‚Ì‚½‚ßtrans‚É‚µ‚Æ‚­
		gEngine->EndPostEffect();
	}

	void PostEffect::ToggleRenderTargetNum()
	{
		m_renderTargetNum ^= 1;
	}
	RenderTarget& PostEffect::GetCurrentRenderTarget()
	{
		return m_renderTarget[m_renderTargetNum];
	}
	void PostEffect::DrawPrimitive()
	{
		m_primitive.Draw();
	}
	Primitive* PostEffect::GetPrimitive()
	{
		return &m_primitive;
	}
}