#pragma once


namespace UsualEngine
{
	class PostEffect;

	/// <summary>
	/// 
	/// </summary>
	class RayMarchingFog
	{
	public:
		RayMarchingFog();
		~RayMarchingFog();

		/// <summary>
		/// 
		/// </summary>
		void Init();
		/// <summary>
		/// 
		/// </summary>
		void Release();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pe"></param>
		void Render(PostEffect* pe);
	private:
		struct CBData
		{
			CMatrix mVP;
			CMatrix mVPI;
			CVector4 mainLightColor;
			CVector3 mainLightDir;
			CVector3 camPos;
			float camFar;
			float camNear;
			float offset;
		};
		ConstantBuffer m_cBuffer;
		CBData m_cbData;
		Shader m_ps;

		RenderTarget m_rtFog;
		RenderTarget m_rtVolume;
		
		float m_offsetSpeed = 100.f;
		float m_maxOffset = 1000.f;
	};
}