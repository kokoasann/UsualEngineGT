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
		void DrawStart(const CVector4& clearColor = {0,0,0,0});
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
		GaussianBlur m_gausBlur_mid;
		float m_blurParam = 1;
		float m_blurParamReal = 1;

		
		Shader m_vsCopy;
		Shader m_psCopy;
	};
}