#include "PreCompile.h"
#include "SkinModelRender4Ground.h"


namespace UsualEngine
{
	void SkinModelRender4Ground::InitG(const wchar_t* path, AnimationClip* anims, int animCount, EnFbxUpAxis axis)
	{
		Init(path, anims, animCount, axis);
		
		m_psShader.Load("Assets/shader/model.fx","PSMain_Ground",Shader::EnType::PS);
		GetSkinModel().FindMaterial([&](ModelEffect* mate)
		{
			mate->SetPSShader(m_psShader);
		});
		m_groundCB.Create(&m_goundData, sizeof(m_goundData));
	}
	void SkinModelRender4Ground::Render()
	{
		m_goundData.scale = GetSca();
		CVector3 dir = {0,0,1};
		GetRot().Multiply(dir);
		m_goundData.dir = dir;

		auto dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		dc->UpdateSubresource(m_groundCB.GetBody(), 0, NULL, (const GroundCB*)& m_goundData, 0, 0);
		dc->PSSetConstantBuffers(enSkinModelCBReg_Ground, 1, &m_groundCB.GetBody());

		Camera& cam = usualEngine()->GetMainCamera();
		GetSkinModel().Draw(cam.GetViewMatrix(), cam.GetProjectionMatrix());
	}
}