#pragma once


namespace UsualEngine
{
	class PostEffect;

	/// <summary>
	/// 
	/// </summary>
	class RayMarchingFog
	{
	public:
		RayMarchingFog();
		~RayMarchingFog();

		/// <summary>
		/// �������B
		/// </summary>
		void Init();
		/// <summary>
		/// �J���B
		/// </summary>
		void Release();

		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="pe"></param>
		void Render(PostEffect* pe);
	private:
		//�萔�o�b�t�@�̒l
		struct CBData
		{
			CMatrix mVP;				//�r���[�v���W�F�N�V����
			CMatrix mVPI;				//�t�r���[�v���W�F�N�V����
			CVector4 mainLightColor;	//���C�����C�g�̐F�B
			CVector3 mainLightDir;		//���C�����C�g�̕����B
			float NONE = 0;
			CVector3 camPos;			//�J�����̈ʒu�B
			float offset;				//�I�t�Z�b�g
			float camFar;
			float camNear;
		};
		ConstantBuffer m_cBuffer;		//�萔�o�b�t�@
		CBData m_cbData;				//�萔�o�b�t�@�̃f�[�^
		Shader m_ps;		//�s�N�Z���V�F�[�_

		RenderTarget m_rtFog;		//�t�H�O�p��RT
		RenderTarget m_rtVolume;	//LightVolume�p��RT
		GaussianBlur m_blur;		//�u���[
		
		float m_offsetSpeed = 100.f;	//�I�t�Z�b�g�X�s�[�h
		float m_maxOffset = 1000.f;		//�I�t�Z�b�g�̍ő�l�B
	};
}