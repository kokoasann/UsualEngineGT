#pragma once
#include "DebugLog.h"

namespace UsualEngine
{
	/// <summary>
	/// シングルトン
	/// デバッグ用のクラス
	/// </summary>
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
		/// <summary>
		/// debugSwitchの初期化。
		/// </summary>
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
			gdSpecular,
		};
		GBufferDraw gbufferDraw = gdDefault;

		/// <summary>
		/// DebugLogに表示する文字を入力。
		/// </summary>
		/// <param name="str"></param>
		void DebugPrint(const wchar_t* str);
		/// <summary>
		/// ログの描画。
		/// </summary>
		void DrawLog();
	private:
		DebugLog m_log;		//デバッグログ
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