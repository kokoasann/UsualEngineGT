#pragma once
#include "Bloom.h"

namespace UsualEngine
{
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
	private:
		Primitive m_primitive;
		int m_renderTargetNum = 0;			//作業中のレンダーターゲット
		RenderTarget m_renderTarget[2];		//レンダーターゲットの配列。
		Bloom m_bloom;									//ブルーム
	};
}