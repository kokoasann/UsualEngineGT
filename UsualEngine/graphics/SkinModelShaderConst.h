/*!
 *@brief	�X�L�����f���֌W�֌W�̃V�F�[�_�[�p�����[�^�B
 */

#pragma once

namespace UsualEngine
{
	/*!
	 *@brief	�萔�o�b�t�@�̃��W�X�^�ԍ�
	 */
	enum EnSkinModelCBReg {
		enSkinModelCBReg_VSPS,		//!<���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���ʂ̒萔�o�b�t�@�B
		enSkinModelCBReg_Light,		//!<���C�g�p�̒萔�o�b�t�@�B
		enSkinModelCBReg_Material,	//!<�}�e���A���p�����[�^�B
		enSkinModelCBReg_Shadow,	//!<�e�B
		enSkinModelCBReg_GBuffer,	//!<G-Buffer�B
		enSkinModelCBReg_Ground,	//�n�ʗp�̒萔�o�b�t�@
	};
	/*!
	 *@brief	SRV�̃��W�X�^�ԍ��B
	 */
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_AlbedoTexture,		//!<�A���x�h�e�N�X�`���B
		enSkinModelSRVReg_BoneMatrixArray,		//!<�{�[���s��̔z��B
		enSkinModelSRVReg_ShadowMap_1,			//�V���h�E�}�b�v
		enSkinModelSRVReg_ShadowMap_MAXNUM= enSkinModelSRVReg_ShadowMap_1+ MAX_SHADOW_MAP-1,			//�V���h�E�}�b�v
		enSkinModelSRVReg_DirectionLight=100,		//�f�B���N�V�������C�g
		enSkinModelSRVReg_BlendMap,			//�e�N�X�`���ƃe�N�X�`���������邽�߂̃e�N�X�`��
		enSkinModelSRVReg_Textur_1,				//���ړI�e�N�X�`��(���Ԃ�)
		enSkinModelSRVReg_Textur_2,				//���ړI�e�N�X�`��(���Ԃ�)
		enSkinModelSRVReg_Textur_3,				//���ړI�e�N�X�`��(���Ԃ�)
	};
}