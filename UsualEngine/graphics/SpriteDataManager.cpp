#include "PreCompile.h"
#include "SpriteDataManager.h"

namespace UsualEngine
{
	SpriteDataManager::~SpriteDataManager()
	{
		Release();
	}
	void SpriteDataManager::Release()
	{
		mDatas.clear();
		
	}
	ID3D11ShaderResourceView* SpriteDataManager::Load(std::wstring& st)
	{
		ID3D11ShaderResourceView* srv;
		auto it = mDatas.find(st);
		if (it == mDatas.end())
		{
			
			HRESULT res = DirectX::CreateDDSTextureFromFileEx(usualEngine()->GetGraphicsEngine()->GetD3DDevice, st.c_str(), 0,
				D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
				false, nullptr, &srv);
			mDatas.insert({ st, std::move(srv) });
		}
		else
		{
			srv = it->second;
		}
		return srv;
	}
}