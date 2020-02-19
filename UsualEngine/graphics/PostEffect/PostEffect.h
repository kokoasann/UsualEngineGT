#pragma once
#include "Bloom.h"
#include "AlphaModel.h"
#include "../Primitive.h"
namespace UsualEngine
{

	class PostEffect
	{
	public:
		PostEffect();
		~PostEffect();
		void Init();
	private:
		/// <summary>
		/// renderTarget������������
		/// </summary>
		void InitRenderTarget();
		/// <summary>
		/// primitive������������
		/// </summary>
		void InitPrimitive();
	public:
		/// <summary>
		/// �`�悵�܂���
		/// </summary>
		void Render();

		/// <summary>
		/// RenderTarget��؂�ւ���
		/// </summary>
		void ToggleRenderTargetNum();
		/// <summary>
		/// ��ƒ��̃����_�[�^�[�Q�b�g��Ԃ�
		/// </summary>
		/// <returns></returns>
		RenderTarget& GetCurrentRenderTarget();
		void DrawPrimitive();

		Primitive* GetPrimitive();
		AlphaModelRender& GetAlphaBlendRender()
		{
			return m_alphaModelRender;
		}
	private:
		Primitive m_primitive;
		int m_renderTargetNum = 0;			//��ƒ��̃����_�[�^�[�Q�b�g
		RenderTarget m_renderTarget[2];		//�����_�[�^�[�Q�b�g�̔z��B
		Bloom m_bloom;									//�u���[��
		AlphaModelRender m_alphaModelRender;
	};
}