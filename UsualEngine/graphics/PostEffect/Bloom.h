#pragma once
#include "GaussianBlur.h"


//ダウンサンプリングする数
#define BLOOM_NUM_DWNSMP 5
namespace UsualEngine
{
	class PostEffect;

	class Bloom
	{
	public:
		Bloom();
		~Bloom();
		void Init();
		void Release();

		void Render(PostEffect* pe);
	private:
		Shader m_vs;				//頂点シェーダ
		Shader m_ps;				//ピクセルシェーダ
		Shader m_psCombine;			//ダウンサンプリングの合成用のピクセルシェーダ
		Shader m_vsCopy;			//コピー用VS
		Shader m_psCopy;			//コピー用PS
		RenderTarget m_luminanceRT;			//光度抽出用のRT
		RenderTarget m_combineRT;			//合成用のRT
		GaussianBlur m_gausBlur[BLOOM_NUM_DWNSMP];		//ガウスブラー
	};
}