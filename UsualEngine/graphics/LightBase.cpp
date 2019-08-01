#include "PreCompile.h"
#include "LightBase.h"
#include "LightManager.h"

namespace UsualEngine
{
	bool LightBase::Start()
	{
		usualEngine()->GetGraphicsEngine()->GetLightManager().AddLight(this);
		return true;
	}
	void LightBase::OnDestroy()
	{
		usualEngine()->GetGraphicsEngine()->GetLightManager().RmvLight(this);
	}
}