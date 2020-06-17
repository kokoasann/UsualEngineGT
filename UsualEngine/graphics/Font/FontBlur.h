#pragma once
#include "graphics/RenderTarget.h"
#include "graphics/PostEffect/GaussianBlur.h"

namespace UsualEngine
{
	class FontBlur
	{
	public:
		FontBlur();
		~FontBlur();
		void Init();
		void DrawStart();
		void DrawEnd();
		void Reset()
		{
			m_blurParam = 0;
		}
		void SetBlurParam(float p)
		{
			m_blurParam = p;
		}
		float GetBlurParam() const
		{
			return m_blurParam;
		}
	private:
		RenderTarget m_renderTarget;
		RenderTarget* m_oldRenderTarget[7];
		int m_oldRenderNum = 0;
		GaussianBlur m_gausBlur;
		float m_blurParam = 0;
	};
}