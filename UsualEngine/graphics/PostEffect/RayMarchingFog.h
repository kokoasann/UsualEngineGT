#pragma once


namespace UsualEngine
{
	class PostEffect;
	class RayMarchingFog
	{
	public:
		RayMarchingFog();
		~RayMarchingFog();

		void Init();
		void Release();

		void Render(PostEffect* pe);
	private:
		struct CBData
		{
			CMatrix mVP;
			CMatrix mVPI;
			CVector3 mainLightDir;
			CVector3 camPos;
			float camFar;
			float camNear;
			float offset;
		};
		ConstantBuffer m_cBuffer;
		CBData m_cbData;
		Shader m_ps;
		
		float m_offsetSpeed = 100.f;
		float m_maxOffset = 1000.f;
	};
}