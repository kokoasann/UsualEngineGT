#include "PreCompile.h"
#include "LightManager.h"
#include "LightBase.h"
#include "LightDirection.h"


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
		InitDirectionStructuredBuffet();
		mLightParamCB.Create(&mLightParam, sizeof(mLightParam));
	}

	void LightManager::InitDirectionStructuredBuffet()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		int stride = sizeof(SDirectionLight);

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRVとしてバインド可能。
		desc.ByteWidth = stride * MAX_DIRLIGHT;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = stride;

		mDirLightSB.Create(NULL, desc);
	}

	void LightManager::AddLight(LightBase* light)
	{
		const std::type_info& type = typeid(*light);
		if (type == typeid(LightDirection))
		{
			if (MAX_DIRLIGHT <= mCDirLight.size())
				return;
			auto it = std::find(mCDirLight.begin(), mCDirLight.end(), light);
			if (it == mCDirLight.end())
			{
				mCDirLight.push_back(dynamic_cast<LightDirection*>(light));
			}
		}
	}
	void LightManager::RmvLight(LightBase* light)
	{
		const std::type_info& type = typeid(*light);
		if (type == typeid(LightDirection))
		{
			mCDirLight.erase(std::remove(mCDirLight.begin(), mCDirLight.end(), light), mCDirLight.end());
		}
	}
	void LightManager::Update()
	{
		int cunt = 0;
		for (auto L : mCDirLight)
		{
			if (!L->IsActive())
				continue;
			mSDirLights[cunt] = L->GetBody();
			cunt++;
		}
		mLightParam.DLCount = cunt;
		mLightParam.eyePos = usualEngine()->GetMainCamera().GetPosition();
		mLightParam.screen.x = 0;
		mLightParam.screen.y = 0;
		mLightParam.screen.z = FRAME_BUFFER_W;
		mLightParam.screen.w = FRAME_BUFFER_H;
	}

	void LightManager::Render()
	{
		ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();

		dc->UpdateSubresource(mDirLightSB.GetBody(), 0, NULL, mSDirLights, 0, 0);

		dc->UpdateSubresource(mLightParamCB.GetBody(), 0, NULL, &mLightParam, 0, 0);

		dc->PSSetShaderResources(enSkinModelSRVReg_DirectionLight, 1,&mDirLightSB.GetSRV());	//ディレクションライトをセット
		dc->PSSetConstantBuffers(enSkinModelCBReg_Light, 1, &mLightParamCB.GetBody());		//ライトの情報を送る
	}
	void LightManager::EndRender()
	{
		ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		ID3D11ShaderResourceView* view[] = { NULL };
		dc->PSSetShaderResources(enSkinModelSRVReg_DirectionLight, 1, view);//NULLで上書き
	}
}