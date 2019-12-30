#include "PreCompile.h"
#include "SoftShadow.h"
#include "graphics/PreRender.h"
#include "graphics/GBuffer.h"

namespace UsualEngine
{
	SoftShadow::SoftShadow()
	{
	}
	SoftShadow::~SoftShadow()
	{
		m_shadowRT.Release();
		m_gauss.Release();
	}
	void SoftShadow::Init()
	{
		DXGI_SAMPLE_DESC msaaD;
		ZeroMemory(&msaaD, sizeof(msaaD));
		msaaD.Count = 1;
		msaaD.Quality = 0;
		//m_shadowRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_D16_UNORM, msaaD);
		m_gauss.Init(FRAME_BUFFER_W, FRAME_BUFFER_H);
		m_gauss.SetDispersion(0.0001f);
		
		m_gBufShadow =  usualEngine()->GetGraphicsEngine()->GetPreRender().GetGBuffer().GetGBuffer(GBuffer::GB_Shadow);
	}
	ID3D11ShaderResourceView* SoftShadow::Draw()
	{
		auto& pe = usualEngine()->GetGraphicsEngine()->GetPostEffect();
		auto shadow = m_gauss.Render(m_gBufShadow->GetSRV(), pe.GetPrimitive());
		return shadow;
	}
}