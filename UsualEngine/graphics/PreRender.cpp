#include "PreCompile.h"
#include "PreRender.h"


namespace UsualEngine
{
	void PreRender::Init(float h, float w)
	{
		m_gBuffer.Init(h, w);
		m_softShadow.Init();
		m_constBuffer.Create(&m_cbData, sizeof(m_cbData));
	}
	void PreRender::Render()
	{
		m_gBuffer.Draw();
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