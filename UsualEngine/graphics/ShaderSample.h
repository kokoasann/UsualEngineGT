#pragma once
#include "Shader.h"

namespace UsualEngine
{
	/// <summary>
	/// �悭�g���V�F�[�_�����I���Ē񂵂܂����B
	/// </summary>
	struct ShaderSample
	{
		/// <summary>
		/// ������
		/// </summary>
		static void Init();
		static Shader VS_Model;			//�X�L���Ȃ����f��
		static Shader VS_Model_Skin;	//�X�L�����胂�f��
		static Shader PS_Model;			//���f��PS(�f�t�@�[�h�p)
		static Shader PS_AlphaModel;	//�A���t�@���f���p�B
		static Shader PS_AlphaModel_Specular;	//�A���t�@���f���@�X�y�L����

		static Shader VS_Sprite;		//�X�v���C�gVS
		static Shader PS_Sprite;		//�X�v���C�gPS

		static Shader VS_Copy;			//�R�s�[VS
		static Shader PS_Copy;			//�R�s�[PS
	};
}