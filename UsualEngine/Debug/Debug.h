#pragma once

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
	private:
		
	};
}