#include "PreCompile.h"
#include "SkinModelRender4Ground.h"


namespace UsualEngine
{
	void SkinModelRender4Ground::Release()
	{
		SkinModelRender::Release();
		m_blendMap->Release();
		m_groundCB.Release();
		m_psShader.Release();
		for (int n = 0; n < 3; n++)
		{
			if (m_textures[n]!=nullptr)
			{
				//m_textures[n]->Release();
				m_textures[n] = nullptr;
			}
		}
		for (auto& sp : m_specularMap)
		{
			if (sp != nullptr)
				sp = nullptr;
		}

	}
	void SkinModelRender4Ground::InitG(const wchar_t* path, AnimationClip* anims, int animCount, EnFbxUpAxis axis, GroundType groundType, bool isUseHightMap)
	{
		Init(path, anims, animCount, axis);
		switch (groundType)
		{
		case gtDefault:
			m_psShader.Load("Assets/shader/model.fx", "PSMain_GroundDefault", Shader::EnType::PS);
			break;
		case gtBlendTow:
			m_psShader.Load("Assets/shader/model.fx", "PSMain_GroundBlendTwo", Shader::EnType::PS);
			break;
		case gtBlendThree:
			m_psShader.Load("Assets/shader/model.fx", "PSMain_GroundBlendThree", Shader::EnType::PS);
			break;
		}
		if (isUseHightMap)
		{
			m_vsShader.Load("Assets/shader/model.fx", "VSMain_Ground", Shader::EnType::VS);
		}
		else
		{
			m_vsShader.Load("Assets/shader/model.fx", "VSMain_Ground", Shader::EnType::VS);
		}
		GetSkinModel().FindMaterial([&](ModelEffect* mate)
		{
			mate->SetPSShader(m_psShader);
			mate->SetVSShader(m_vsShader);
		});
		m_groundCB.Create(&m_groundData, sizeof(m_groundData));
		m_groundCB_VS.Create(&m_groundVSData, sizeof(m_groundVSData));
	}

	void SkinModelRender4Ground::SetBlendMap(const wchar_t* path)
	{
		//std::wstring st = path;
		m_blendMap = SpriteDataManager::Get()->Load(path);
	}
	void SkinModelRender4Ground::SetTexture(int num,const wchar_t* path, const wchar_t* materialName)
	{
		//std::wstring st = path;
		m_textures[num] = SpriteDataManager::Get()->Load(path);
		const wchar_t* matename = materialName;
		if (materialName == nullptr)
		{
			switch (num)
			{
			case tkGrass:
				matename = m_grassName.c_str();
				break;
			case tkTuti:
				matename = m_tutiName.c_str();
				break;
			case tkOther:
				matename = m_otherName.c_str();
				break;
			}
		}
		GetSkinModel().FindMaterial([&](ModelEffect* mate)
		{
			if (mate->EqualMaterialName(matename))
			{
				mate->SetAlbedoTexture(m_textures[num]);
			}
		});
	}

	void SkinModelRender4Ground::SetSpecularMap(int num, const wchar_t* path,const wchar_t* materialName)
	{
		//std::wstring st = path;
		m_specularMap[num] = SpriteDataManager::Get()->Load(path);
		const wchar_t* matename = materialName;
		if (materialName == nullptr)
		{
			switch (num)
			{
			case tkGrass:
				matename = m_grassName.c_str();
				break;
			case tkTuti:
				matename = m_tutiName.c_str();
				break;
			case tkOther:
				matename = m_otherName.c_str();
				break;
			}
		}
		GetSkinModel().FindMaterial([&](ModelEffect* mate)
			{
				if (mate->EqualMaterialName(matename))
				{
					mate->SetSpecularMap(m_specularMap[num]);
				}
			});
	}

	void SkinModelRender4Ground::SetHightMap(const wchar_t* path)
	{
		//std::wstring p = path;
		m_hightMap = SpriteDataManager::Get()->Load(path);
	}

	void SkinModelRender4Ground::Render()
	{
		m_groundData.scale = GetSca();
		CVector3 dir = {0,0,1};
		CQuaternion rot = GetRot();
		rot=rot*-1;
		rot.w *= -1;
		m_groundData.dir.MakeRotationFromQuaternion(rot);

		auto dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();

		int isUseTex[4] = { 0 };
		int isUseSpe[4] = { 0 };
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
		isUseSpe[0] = m_specularMap[0] != nullptr;
		for (int i = 0; i < 3; i++)
		{
			if (m_specularMap[i] != nullptr)
			{
				dc->PSSetShaderResources(enSkinModelSRVReg_Specular_1 + i, 1, &m_specularMap[i]);
				isUseSpe[i + 1] = 1;
			}
		}

		if (m_hightMap != nullptr)
		{
			dc->VSSetShaderResources(enSkinModelSRVReg_HightMap_1, 1, &m_hightMap);
			m_groundVSData.isUseHightMap = 1;
		}
		else
			m_groundVSData.isUseHightMap = 0;

		m_groundData.isUseTextures.x = isUseTex[0];
		m_groundData.isUseTextures.y = isUseTex[1];
		m_groundData.isUseTextures.z = isUseTex[2];
		m_groundData.isUseTextures.w = isUseTex[3];

		m_groundData.isUseSpecular.x = isUseSpe[0];
		m_groundData.isUseSpecular.y = isUseSpe[1];
		m_groundData.isUseSpecular.z = isUseSpe[2];
		m_groundData.isUseSpecular.w = isUseSpe[3];

		//PS用の定数バッファを送信。
		dc->UpdateSubresource(m_groundCB.GetBody(), 0, NULL, (const GroundCB*)& m_groundData, 0, 0);
		dc->PSSetConstantBuffers(enSkinModelCBReg_Ground_PS, 1, &m_groundCB.GetBody());

		//VS用の定数バッファを送信。
		dc->UpdateSubresource(m_groundCB_VS.GetBody(), 0, NULL, (const GroundCB_VS*)&m_groundVSData, 0, 0);
		dc->VSSetConstantBuffers(enSkinModelCBReg_Ground_VS, 1, &m_groundCB_VS.GetBody());

		Camera& cam = usualEngine()->GetMainCamera();
		GetSkinModel().Draw(cam.GetViewMatrix(), cam.GetProjectionMatrix());
	}
}