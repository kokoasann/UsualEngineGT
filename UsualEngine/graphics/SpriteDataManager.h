#pragma once
#include <map>

namespace UsualEngine
{
	class SpriteDataManager
	{
		SpriteDataManager() {};
		~SpriteDataManager();
	public:
		static SpriteDataManager* Get()
		{
			static SpriteDataManager* inst = NULL;
			if (inst == NULL)
				inst = new SpriteDataManager();
			return inst;
		}
		void Release();

		//ID3D11ShaderResourceView* Load(std::wstring& st);
		ID3D11ShaderResourceView* Load(const wchar_t* st);

	private:
		std::map<std::wstring, ID3D11ShaderResourceView*> mDatas;
	};
}