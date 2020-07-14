#pragma once
#include "Shader.h"

namespace UsualEngine
{
	struct ShaderSample
	{
		static void Init();
		static Shader VS_Model;
		static Shader VS_Model_Skin;
		static Shader PS_Model;
		static Shader PS_AlphaModel;
		static Shader PS_AlphaModel_Specular;

		static Shader VS_Sprite;
		static Shader PS_Sprite;

		static Shader VS_Copy;
		static Shader PS_Copy;
	};
}