#include "PreCompile.h"
#include "SkyRender.h"


namespace UsualEngine
{
	SkyRender::SkyRender()
	{
	}
	SkyRender::~SkyRender()
	{
		Release();
	}
	void SkyRender::Init()
	{
		m_ps.Load("Assets/Shader/Sky.fx", "PSMain_Sky", Shader::EnType::PS);
		m_sky.Init(L"Assets/model/BackGroundTest.cmo");
		m_sky.FindMaterial([&](ModelEffect* mate)
		{
			mate->SetPSShader(m_ps);
		});
		CMatrix mat;
		mat.MakeScaling(CVector3::One()*20.f);
		m_sky.UpdateWorldMatrix(mat);
	}
	void SkyRender::Release()
	{
	}
	void SkyRender::Render(PostEffect* pe)
	{

		auto& cam = usualEngine()->GetMainCamera();
		m_sky.Draw(cam.GetViewMatrix(),cam.GetProjectionMatrix());
	}
	void SkyRender::Render()
	{
		auto& cam = usualEngine()->GetMainCamera();
		m_sky.Draw(cam.GetViewMatrix(), cam.GetProjectionMatrix());
	}
}