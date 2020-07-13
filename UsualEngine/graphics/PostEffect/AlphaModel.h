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
		RenderTarget m_depthRenderTarget;
		RenderTarget m_normalRenderTarget;
		std::vector<GameObject*> m_renderObject;
		Shader m_psCopy;
		Shader m_vsCopy;
		Shader m_psDoubleCopy;
	};
}