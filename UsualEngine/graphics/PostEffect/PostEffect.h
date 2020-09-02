#pragma once
#include "Bloom.h"
#include "AlphaModel.h"
#include "SSReflection.h"
#include "../Primitive.h"

#include "../CloudPlane.h"
#include "RayMarchingFog.h"


namespace UsualEngine
{
	class EffectObjectManager;
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
		SSReflection m_ssr;

		CloudPlane m_cloudPlane;
		RayMarchingFog m_rmFog;
		EffectObjectManager* m_effectObjectManager;
	};
}