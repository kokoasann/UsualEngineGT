#pragma once
#include "graphics/RenderTarget.h"
#include "graphics/PostEffect/GaussianBlur.h"

namespace UsualEngine
{
	class FontBlur
	{
	public:
		static float PARAM_MAX;
		static float BASE_SPEED_LIST[3];
		static float BASE_TIME;

		enum EState
		{
			SSTOP,
			SUP,
			SDOWN
		};

		FontBlur();
		~FontBlur();
		void Release();
		void Init();
		void Update();
		void DrawStart(const CVector4& clearColor = {0,0,0,0});
		void DrawEnd();
		void Reset()
		{
			m_blurParam = FLT_EPSILON;
		}
		void SetBlurParam(float p)
		{
			m_blurParam = p;
		}
		float GetBlurParam() const
		{
			return m_blurParam;
		}

		void SetSpeed(float sp)
		{
			m_speed = sp*BASE_TIME;
		}

		/// <summary>
		/// 徐々に鮮明にしていく
		/// </summary>
		void Up()
		{
			m_state = SUP;
		}
		/// <summary>
		/// 徐々にぼかしていく
		/// </summary>
		void Down()
		{
			m_state = SDOWN;
		}
		/// <summary>
		/// 遷移のストップ
		/// </summary>
		void Stop()
		{
			m_state = SSTOP;
		}

		bool IsStop() const
		{
			return m_state == SSTOP;
		}

	private:
		RenderTarget m_renderTarget;
		RenderTarget* m_oldRenderTarget[7] = {0};
		int m_oldRenderNum = 0;
		GaussianBlur m_gausBlur;
		GaussianBlur m_gausBlur_mid;
		GaussianBlur m_gausBlur_sml;
		float m_blurParam = FLT_EPSILON;
		//float m_blurParamReal = 1;

		
		Shader m_vsCopy;
		Shader m_psCopy;

		EState m_state = SSTOP;

		float m_speed = 0.0f; //秒。
		
	};
}