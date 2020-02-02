#pragma once
#include "DebugLog.h"

namespace UsualEngine
{
	class Debug
	{
		Debug(){}
	public:
		~Debug(){}
		static Debug& Instance()
		{
			static Debug ins;
			return ins;
		}
		void InitDebugSwitch();
		struct DebugState
		{
			bool isPhysicsDebugDraw = false;
		};
		DebugState debugState;
		enum GBufferDraw
		{
			gdDefault,
			gdDiffuse,
			gdNormal,
			gdDepth,
			gdShadow,
		};
		GBufferDraw gbufferDraw = gdDefault;

		void DebugPrint(const wchar_t* str);
		void DrawLog();
	private:
		DebugLog m_log;
	};
	/// <summary>
	/// デバッグプリント
	/// </summary>
	/// <param name="str"></param>
	static void DebugPrint(const wchar_t* str)
	{
		Debug::Instance().DebugPrint(str);
	}
}