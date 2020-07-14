#pragma once


namespace UsualEngine
{
	class PostEffect;
	class GameObject;

	//�������I�u�W�F�N�g�̕`����s�����߂̃N���X�B
	class AlphaModelRender
	{
	public:
		/// <summary>
		/// �������B
		/// </summary>
		void Init();
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="posteffect"></param>
		void Render(PostEffect* posteffect);

		/// <summary>
		/// �������ރ��f���̒ǉ��B
		/// </summary>
		/// <param name="go"></param>
		void AddRenderObject(GameObject* go)
		{
			m_renderObject.push_back(go);
		}
	private:
		RenderTarget m_alphaRenderTarget;			//���f������������RT
		RenderTarget m_depthRenderTarget;			//GBuffer�̐[�x�l���R�s�[���邽�߂�RT
		RenderTarget m_normalRenderTarget;			//GBuffer�̖@�����R�s�[���邽�߂�RT
		RenderTarget m_specularRenderTarget;		//GBuffer�̃X�y�L�������R�s�[���邽�߂�RT
		std::vector<GameObject*> m_renderObject;	//�������ރ��f���B�B
		Shader m_psCopy;			//�ŏI�����pPS�R�s�[�B
		Shader m_vsCopy;			//�R�s�[�pVS
		Shader m_psTripleCopy;		//GBuffer�R�s�[�p(��C��3�R�s�[���邼�I)

	};
}