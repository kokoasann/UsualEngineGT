#pragma once


namespace UsualEngine
{
	class PostEffect;
	class GameObject;
	class AlphaModelRender
	{
	public:
		void Init();
		void Render(PostEffect* posteffect);

		void AddRenderObject(GameObject* go)
		{
			m_renderObject.push_back(go);
		}
	private:
		RenderTarget m_alphaRenderTarget;
		std::vector<GameObject*> m_renderObject;
		Shader m_psCopy;
		Shader m_vsCopy;
	};
}