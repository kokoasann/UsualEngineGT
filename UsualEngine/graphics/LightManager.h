#pragma once

#include "graphics/ConstantBuffer.h"
#include "graphics/StructuredBuffer.h"
#include "LightStruct.h"


namespace UsualEngine
{
#define MAX_DIRLIGHT 8
	class LightBase;
	class LightDirection;

	/*
	���C�g���Ǘ�������
	*/
	class LightManager
	{
	friend class GraphicsEngine;
	private:
		LightManager();
		~LightManager();
	public:
		////�����̃C���X�^���X���Q�b�g�I�I
		//static LightManager* Get()
		//{
		//	static LightManager ins;
		//	
		//	return &ins;
		//}

		//������
		void Init();

		void InitDirectionStructuredBuffet();

		/*
		���C�g�ǉ�
		*/
		void AddLight(LightBase* light);

		/*
		���C�g����
		*/
		void RmvLight(LightBase* light);

		//�X�V
		void Update();

		//�`��
		void Render();

		//�`��I��
		void EndRender();

		/// <summary>
		/// 0�Ԗڂ̃f�B���N�V�������C�g��Ԃ�
		/// </summary>
		/// <returns></returns>
		LightDirection* GetMainLightDirection()
		{
			if (mCDirLight.size() > 0)
			{
				return mCDirLight[0];
			}
			else
			{
				return nullptr;
			}
		}
		//���C�g�̃p�����[�^�̍\����
		struct LightParam
		{
			CVector3 eyePos = CVector3::Zero();		//���C���J�����̈ʒu
			int DLCount = 0;						//�f�B���N�V�������C�g�̐�
			CVector4 screen;						//�X�N���[���̃T�C�Y
		};
	private:
		LightParam mLightParam;						//���C�g�̏��
		ConstantBuffer mLightParamCB;				//���C�g�p�����[�^�̒萔�o�b�t�@
		std::vector<LightDirection*> mCDirLight;	//�f�B���N�V�������C�g�̃��X�g
		SDirectionLight mSDirLights[MAX_DIRLIGHT];	//�f�B���N�V�������C�g�\���̂̃��X�g
		StructuredBuffer mDirLightSB;				//�f�B���N�V�������C�g��StructuredBuffer
	};
}