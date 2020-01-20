#include "PreCompile.h"
#include "Debug.h"


namespace UsualEngine
{
	void Debug::InitDebugSwitch()
	{
		std::function<void()> on = [&]()
		{
			debugState.isPhysicsDebugDraw = true;
		};
		std::function<void()> off = [&]()
		{
			debugState.isPhysicsDebugDraw = false;
		};
		DebugSwitchAddCheck(DebugSwitchNewSwitch(VK_F1, 0, on, off));


#if _DEBUG
		DebugSwitchNewRadioBox("GBufferDraw");
		on = [&]()
		{
			gbufferDraw = gdDefault;
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F2, 0, on));
		on = [&]()
		{
			gbufferDraw = gdDiffuse;
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F3, 0, on));
		on = [&]()
		{
			gbufferDraw = gdNormal;
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F4, 0, on));
		on = [&]()
		{
			gbufferDraw = gdDepth;
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F5, 0, on));
		on = [&]()
		{
			gbufferDraw = gdShadow;
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F6, 0, on));
#endif
	}
}