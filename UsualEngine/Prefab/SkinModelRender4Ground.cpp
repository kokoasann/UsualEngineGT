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

	void SkinModelRender4Ground::SetBlendMap(const wchar_t* path)
	{
		std::wstring st = path;
		m_blendMap = SpriteDataManager::Get()->Load(st);
	}
	void SkinModelRender4Ground::SetTexture(int num,const wchar_t* path)
	{
		std::wstring st = path;
		m_textures[num] = SpriteDataManager::Get()->Load(st);
	}

	void SkinModelRender4Ground::Render()
	{
		m_goundData.scale = GetSca();
		CVector3 dir = {0,0,1};
		CQuaternion rot = GetRot();
		rot=rot*-1;
		rot.w *= -1;
		m_goundData.dir.MakeRotationFromQuaternion(rot);

		auto dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();

		int isUseTex[4] = { 0 };
		if (m_blendMap != nullptr)
		{
			dc->PSSetShaderResources(enSkinModelSRVReg_BlendMap, 1, &m_blendMap);
			isUseTex[0] = 1;
			for (int i = 0; i < 3; i++)
			{
				if (m_textures[i] != nullptr)
				{
					dc->PSSetShaderResources(enSkinModelSRVReg_Textur_1 + i, 1, &m_textures[i]);
					isUseTex[i+1] = 1;
				}
			}
		}
		m_goundData.isUseTextures.x = isUseTex[0];
		m_goundData.isUseTextures.y = isUseTex[1];
		m_goundData.isUseTextures.z = isUseTex[2];
		m_goundData.isUseTextures.w = isUseTex[3];

		dc->UpdateSubresource(m_groundCB.GetBody(), 0, NULL, (const GroundCB*)& m_goundData, 0, 0);
		dc->PSSetConstantBuffers(enSkinModelCBReg_Ground, 1, &m_groundCB.GetBody());

		Camera& cam = usualEngine()->GetMainCamera();
		GetSkinModel().Draw(cam.GetViewMatrix(), cam.GetProjectionMatrix());
	}
}