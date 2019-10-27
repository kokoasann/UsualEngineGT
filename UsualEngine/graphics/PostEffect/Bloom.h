#pragma once
#include "GaussianBlur.h"

//ダウンサンプリングする数
#define BLOOM_NUM_DWNSMP 10
namespace UsualEngine
{
	class Bloom
	{
	public:
		
	private:
		RenderTarget m_luminanceRT;
		RenderTarget m_combineRT;
		GaussianBlur m_gausBlur[BLOOM_NUM_DWNSMP];		//ガウスブラー
	};
}