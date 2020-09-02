#pragma once
#include "Bloom.h"
#include "AlphaModel.h"
#include "SSReflection.h"
#include "../Primitive.h"

#include "../CloudPlane.h"
#include "RayMarchingFog.h"


namespace UsualEngine
{
	class EffectObjectManager;
	class PostEffect
	{
	public:
		PostEffect();
		~PostEffect();
		void Init();
	private:
		/// <summary>
		/// renderTargetを初期化する
		/// </summary>
		void InitRenderTarget();
		/// <summary>
		/// primitiveを初期化する
		/// </summary>
		void InitPrimitive();
	public:
		/// <summary>
		/// 描画しますん
		/// </summary>
		void Render();

		/// <summary>
		/// RenderTargetを切り替える
		/// </summary>
		void ToggleRenderTargetNum();
		/// <summary>
		/// 作業中のレンダーターゲットを返す
		/// </summary>
		/// <returns></returns>
		RenderTarget& GetCurrentRenderTarget();
		void DrawPrimitive();

		Primitive* GetPrimitive();
		AlphaModelRender& GetAlphaBlendRender()
		{
			return m_alphaModelRender;
		}
	private:
		Primitive m_primitive;
		int m_renderTargetNum = 0;			//作業中のレンダーターゲット
		RenderTarget m_renderTarget[2];		//レンダーターゲットの配列。
		Bloom m_bloom;									//ブルーム
		AlphaModelRender m_alphaModelRender;
		SSReflection m_ssr;

		CloudPlane m_cloudPlane;
		RayMarchingFog m_rmFog;
		EffectObjectManager* m_effectObjectManager;
	};
}