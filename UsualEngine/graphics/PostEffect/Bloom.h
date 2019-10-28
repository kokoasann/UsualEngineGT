#pragma once
#include "GaussianBlur.h"


//�_�E���T���v�����O���鐔
#define BLOOM_NUM_DWNSMP 5
namespace UsualEngine
{
	class PostEffect;

	class Bloom
	{
	public:
		Bloom();
		~Bloom();
		void Init();
		void Release();

		void Render(PostEffect* pe);
	private:
		Shader m_vs;				//���_�V�F�[�_
		Shader m_ps;				//�s�N�Z���V�F�[�_
		Shader m_psCombine;			//�_�E���T���v�����O�̍����p�̃s�N�Z���V�F�[�_
		Shader m_vsCopy;			//�R�s�[�pVS
		Shader m_psCopy;			//�R�s�[�pPS
		RenderTarget m_luminanceRT;			//���x���o�p��RT
		RenderTarget m_combineRT;			//�����p��RT
		GaussianBlur m_gausBlur[BLOOM_NUM_DWNSMP];		//�K�E�X�u���[
	};
}