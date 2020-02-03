#include "PreCompile.h"
#include "LightManager.h"
#include "LightBase.h"
#include "LightDirection.h"
#include "LightPoint.h"


namespace UsualEngine
{
	LightManager::LightManager()
	{
	}
	LightManager::~LightManager()
	{
	}
	void LightManager::Init()
	{
		InitDirectionStructuredBuffer();
		InitPointStructuredBuffer();
		m_lightParamCB.Create(&m_lightParam, sizeof(m_lightParam));
	}

	void LightManager::InitDirectionStructuredBuffer()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		int stride = sizeof(SDirectionLight);

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRVとしてバインド可能。
		desc.ByteWidth = stride * MAX_DIRLIGHT;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = stride;

		m_dirLightSB.Create(NULL, desc);
	}

	void LightManager::InitPointStructuredBuffer()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		int stride = sizeof(SPointLight);

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRVとしてバインド可能。
		desc.ByteWidth = stride * MAX_PNTLIGHT;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = stride;

		m_pntLightSB.Create(NULL, desc);
	}

	void LightManager::AddLight(LightBase* light)
	{
		const std::type_info& type = typeid(*light);
		if (type == typeid(LightDirection))
		{
			if (MAX_DIRLIGHT <= m_cDirLight.size())
				return;
			auto it = std::find(m_cDirLight.begin(), m_cDirLight.end(), light);
			if (it == m_cDirLight.end())
			{
				m_cDirLight.push_back(dynamic_cast<LightDirection*>(light));
			}
		}
		else if (type == typeid(LightPoint))
		{
			if (MAX_PNTLIGHT <= m_cPointLight.size())
				return;
			auto it = std::find(m_cPointLight.begin(), m_cPointLight.end(), light);
			if (it == m_cPointLight.end())
			{
				m_cPointLight.push_back(dynamic_cast<LightPoint*>(light));
			}
		}
	}
	void LightManager::RmvLight(LightBase* light)
	{
		const std::type_info& type = typeid(*light);
		if (type == typeid(LightDirection))
		{
			m_cDirLight.erase(std::remove(m_cDirLight.begin(), m_cDirLight.end(), light), m_cDirLight.end());
		}
		else if (type == typeid(LightPoint))
		{
			m_cPointLight.erase(std::remove(m_cPointLight.begin(), m_cPointLight.end(), light), m_cPointLight.end());
		}
	}
	void LightManager::Update()
	{
		int cunt = 0;
		for (auto L : m_cDirLight)
		{
			if (!L->IsActive())
				continue;
			m_sDirLights[cunt] = L->GetBody();
			cunt++;
		}
		m_lightParam.DLCount = cunt;
		cunt = 0;
		for (auto L : m_cPointLight)
		{
			if (!L->IsActive())
				continue;
			m_sPntLights[cunt] = L->GetBody();
			cunt++;
		}
		m_lightParam.PLCount = cunt;
		m_lightParam.eyePos = usualEngine()->GetMainCamera().GetPosition();
		m_lightParam.screen.x = 0;
		m_lightParam.screen.y = 0;
		m_lightParam.screen.z = FRAME_BUFFER_W;
		m_lightParam.screen.w = FRAME_BUFFER_H;
	}

	void LightManager::Render()
	{
		ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();

		dc->UpdateSubresource(m_dirLightSB.GetBody(), 0, NULL, m_sDirLights, 0, 0);		//ディレクションライトの更新。

		dc->UpdateSubresource(m_pntLightSB.GetBody(), 0, NULL, m_sPntLights, 0, 0);		//ポイントライトの更新。

		dc->UpdateSubresource(m_lightParamCB.GetBody(), 0, NULL, &m_lightParam, 0, 0);	//ライトの情報の更新。

		dc->PSSetShaderResources(enSkinModelSRVReg_DirectionLight, 1,&m_dirLightSB.GetSRV());	//ディレクションライトをセット
		dc->PSSetShaderResources(enSkinModelSRVReg_PointLight, 1,&m_pntLightSB.GetSRV());	//ポイントライトをセット
		dc->PSSetConstantBuffers(enSkinModelCBReg_Light, 1, &m_lightParamCB.GetBody());		//ライトの情報を送る
	}
	void LightManager::EndRender()
	{
		ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		ID3D11ShaderResourceView* view[] = { NULL };
		dc->PSSetShaderResources(enSkinModelSRVReg_DirectionLight, 1, view);//NULLで上書き
		dc->PSSetShaderResources(enSkinModelSRVReg_PointLight, 1, view);
	}
}