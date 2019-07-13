#include "PreCompile.h"
#include "Sprite.h"

#include "SpriteDataManager.h"

namespace UsualEngine
{
	void Sprite::Init(const wchar_t* path, CVector2 size)
	{
		std::wstring st = path;
		mTextuer = SpriteDataManager::Get()->Load(st);

	}
}