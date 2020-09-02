#include "PreCompile.h"
#include "EffectObjectManager.h"


namespace UsualEngine
{
	void EffectObjectManager::Render(PostEffect* pe)
	{
		for (auto eff : m_effectList)
		{
			eff->Render(pe);
		}
	}
}


