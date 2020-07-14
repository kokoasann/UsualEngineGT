#pragma once

//�K�E�X�u���[�ɕK�v�Ȍ��݂̔z��̐�
#define BLUR_NUM_WEIGHT 8
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
	private:
		/// <summary>
		/// 
		/// </summary>
		void UpdateWeight();
	public:

		/// <summary>
		/// �g������́A�r���[�|�[�g�̐ݒ��Y��Ȃ��悤�ɁB
		/// </summary>
		/// <param name="src">texture</param>
		/// <param name="w">width</param>
		/// <param name="h">height</param>
		/// <param name="primitive">primitive</param>
		/// <returns>blured tex</returns>
		ID3D11ShaderResourceView* Render(ID3D11ShaderResourceView* src ,float w, float h, Primitive* primitive);

		/// <summary>
		/// �ڂ������B
		/// </summary>
		/// <param name="v"></param>
		void SetDispersion(float v)
		{
			m_dispersion = v;
			m_isChangeBOKE = true;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetDispersion()
		{
			return m_dispersion;
		}

	private:
		/// <summary>
		/// �萔�o�b�t�@�p�̍\����
		/// </summary>
		struct BlurParam
		{
			CVector4 offset = CVector4::Zero();					//
			float weights[BLUR_NUM_WEIGHT] = {0};		//
			float rttexRatio = 0;
		};
	private:
		RenderTarget m_renderTargetX;	//�����_�[�^�[�Q�b�g �������!!!
		RenderTarget m_renderTargetY;	//�����_�[�^�[�Q�b�g ���C
		Shader m_vsXBlur;			//���p�̒��_�V�F�[�_
		Shader m_vsYBlur;			//�c�p�̒��_�V�F�[�_
		Shader m_psBlur;				//�s�N�Z���V�F�[�_
		BlurParam m_bp;				//constantBuffer�ɓ����f�[�^
		ConstantBuffer m_cb;		//�萔�o�b�t�@
		ID3D11BlendState* m_blendstate = 0;	//�u�����h�X�e�[�g
		float m_dispersion = 25.f;	//�{�P��B
		bool m_isChangeBOKE = true;	//dispersion�̒l���ς�����ꍇtrue�ɂȂ�
	};
}