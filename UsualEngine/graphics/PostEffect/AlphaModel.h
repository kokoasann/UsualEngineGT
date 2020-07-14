#pragma once


namespace UsualEngine
{
	class PostEffect;
	class GameObject;

	//半透明オブジェクトの描画を行うためのクラス。
	class AlphaModelRender
	{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="posteffect"></param>
		void Render(PostEffect* posteffect);

		/// <summary>
		/// 書き込むモデルの追加。
		/// </summary>
		/// <param name="go"></param>
		void AddRenderObject(GameObject* go)
		{
			m_renderObject.push_back(go);
		}
	private:
		RenderTarget m_alphaRenderTarget;			//モデルを書き込むRT
		RenderTarget m_depthRenderTarget;			//GBufferの深度値をコピーするためのRT
		RenderTarget m_normalRenderTarget;			//GBufferの法線をコピーするためのRT
		RenderTarget m_specularRenderTarget;		//GBufferのスペキュラをコピーするためのRT
		std::vector<GameObject*> m_renderObject;	//書き込むモデル達。
		Shader m_psCopy;			//最終合成用PSコピー。
		Shader m_vsCopy;			//コピー用VS
		Shader m_psTripleCopy;		//GBufferコピー用(一気に3つコピーするぞ！)

	};
}