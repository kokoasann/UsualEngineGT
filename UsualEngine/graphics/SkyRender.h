#pragma once


namespace UsualEngine
{
	class PostEffect;
	class SkyRender
	{
	public:
		SkyRender();
		~SkyRender();

		void Init();
		void Release();

		void Render(PostEffect* pe);
		void Render();
	private:
		Shader m_ps;
		SkinModel m_sky;
	};
}