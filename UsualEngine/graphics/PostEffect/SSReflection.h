#pragma once

namespace UsualEngine
{
	class PostEffect;
	class SSReflection
	{
	public:
		SSReflection();
		virtual ~SSReflection();
		/// <summary>
		/// Initialize
		/// </summary>
		void Init();
		/// <summary>
		/// Release
		/// </summary>
		void Release();

		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="pe"></param>
		void Render(PostEffect* pe);
	private:
		struct CBuffer
		{
			//CMatrix projMat;
			//CMatrix invProjMat;
			CMatrix viewMat;	//�J�����r���[�s��B
			CMatrix VPMat;		//�r���[�v���W�F�N�V����
			CMatrix invVPMat;	//�r���[�v���W�F�N�V�����t�s��
			CVector3 campos;	//�J�����̈ʒu
			float camFar;		//���i����
			float camNear;		//�ߌi����
			float rayLen;		//���˂�`�悷�鋗��
			float rayStep;		//1�X�e�b�v�̋���
		};
		Shader m_vs;		//copy vs
		Shader m_ps;		//SSR ps
		Shader m_copyPS;	//copy ps

		ConstantBuffer m_constBuf;	//�萔�o�b�t�@�l�B
		CBuffer m_buf;				//�萔�o�b�t�@


		RenderTarget m_rtBuffer;	//�����_�[�^�[�Q�b�g
		GaussianBlur m_blur;		//�u���[
	};
}