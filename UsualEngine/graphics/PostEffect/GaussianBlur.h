#pragma once

//�K�E�X�u���[�ɕK�v�Ȍ��݂̔z��̐�
#define BLUR_NUM_WEIGHT 9
namespace UsualEngine
{
	class Primitive;
	class GaussianBlur
	{
	public:
		GaussianBlur();
		~GaussianBlur();
		void Init(int w,int h);
		void Release();

		/// <summary>
		/// 
		/// </summary>
		void UpdateWeight();
		/// <summary>
		/// 
		/// </summary>
		/// <param name="src"></param>
		/// <param name="primitive"></param>
		/// <returns></returns>
		ID3D11ShaderResourceView* Render(ID3D11ShaderResourceView* src, Primitive* primitive);

		void SetDispersion(float v)
		{
			m_dispersion = v;
			m_isChangeBOKE = true;
		}
		float GetDispersion()
		{
			return m_dispersion;
		}

		/// <summary>
		/// �萔�o�b�t�@�p�̍\����
		/// </summary>
		struct BlurParam
		{
			CVector4 offset = CVector4::Zero();					//
			float weights[BLUR_NUM_WEIGHT] = {0};		//
		};
	private:
		RenderTarget m_renderTargetX;	//�����_�[�^�[�Q�b�g �������!!!
		RenderTarget m_renderTargetY;	//�����_�[�^�[�Q�b�g ���C
		Shader m_vsXBlur;			//���p�̒��_�V�F�[�_
		Shader m_vsYBlur;			//�c�p�̒��_�V�F�[�_
		Shader m_psBlur;				//�s�N�Z���V�F�[�_
		BlurParam m_bp;				//constantBuffer�ɓ����f�[�^
		ConstantBuffer m_cb;		//�萔�o�b�t�@
		float m_dispersion = 25.f;	//�{�P��B
		bool m_isChangeBOKE = true;	//dispersion�̒l���ς�����ꍇtrue�ɂȂ�
	};
}