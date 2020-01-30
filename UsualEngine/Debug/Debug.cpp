#include "PreCompile.h"
#include "Debug.h"


namespace UsualEngine
{
	void Debug::InitDebugSwitch()
	{
		std::function<void()> on = [&]()
		{
			debugState.isPhysicsDebugDraw = true;
			DebugPrint(L"フィジクスドローON");
			
		};
		std::function<void()> off = [&]()
		{
			debugState.isPhysicsDebugDraw = false;
			DebugPrint(L"フィジクスドローOFF");
		};
		DebugSwitchAddCheck(DebugSwitchNewSwitch(VK_F1, 0, on, off));


#if _DEBUG
		DebugSwitchNewRadioBox("GBufferDraw");
		on = [&]()
		{
			gbufferDraw = gdDefault;
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F2, 0, on,DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdDiffuse;
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F3, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdNormal;
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F4, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdDepth;
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F5, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdShadow;
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F6, 0, on, DebugSwitch::nullFunc));
#endif
	}
	void Debug::DebugPrint(const wchar_t* str)
	{
		m_log.Print(str);
	}
	void Debug::DrawLog()
	{
		m_log.Render();
	}
}