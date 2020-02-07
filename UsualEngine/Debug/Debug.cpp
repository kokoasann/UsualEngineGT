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
			DebugPrint(L"デフォルト");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F2, 0, on,DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdDiffuse;
			DebugPrint(L"G-Buffer ディフューズ");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F3, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdNormal;
			DebugPrint(L"G-Buffer 法線");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F4, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdDepth;
			DebugPrint(L"G-Buffer 深度値");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F5, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdShadow;
			DebugPrint(L"G-Buffer 影");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F6, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdSpecular;
			DebugPrint(L"G-Buffer スペキュラ");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F7, 0, on, DebugSwitch::nullFunc));
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