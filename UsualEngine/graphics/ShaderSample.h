#pragma once
#include "Shader.h"

namespace UsualEngine
{
	/// <summary>
	/// よく使うシェーダを厳選して陳列しました。
	/// </summary>
	struct ShaderSample
	{
		/// <summary>
		/// 初期化
		/// </summary>
		static void Init();
		static Shader VS_Model;			//スキンなしモデル
		static Shader VS_Model_Skin;	//スキンありモデル
		static Shader PS_Model;			//モデルPS(デファード用)
		static Shader PS_AlphaModel;	//アルファモデル用。
		static Shader PS_AlphaModel_Specular;	//アルファモデル　スペキュラ

		static Shader VS_Sprite;		//スプライトVS
		static Shader PS_Sprite;		//スプライトPS

		static Shader VS_Copy;			//コピーVS
		static Shader PS_Copy;			//コピーPS
	};
}