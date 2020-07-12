#pragma once

namespace UsualEngine
{
	class PostEffect;
	class SSReflection
	{
	public:
		SSReflection();
		virtual ~SSReflection();

		void Init();
		void Release();

		void Render(PostEffect* pe);
	private:
		struct CBuffer
		{
			CMatrix projMat;
			CMatrix invProjMat;
			CMatrix viewMat;
			CMatrix VPMat;
			CMatrix invVPMat;
			CVector3 campos;
			float camFar;
			float camNear;
			float rayLen;
			float rayStep;
		};
		Shader m_vs;
		Shader m_ps;
		Shader m_copyPS;

		ConstantBuffer m_constBuf;
		CBuffer m_buf;

		RenderTarget m_rtBuffer;
	};
}