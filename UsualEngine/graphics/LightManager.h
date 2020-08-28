#pragma once

#include "graphics/ConstantBuffer.h"
#include "graphics/StructuredBuffer.h"
#include "graphics/Shader.h"
#include "LightStruct.h"


namespace UsualEngine
{
#define MAX_DIRLIGHT 8
#define MAX_PNTLIGHT 32
	class LightBase;
	class LightDirection;
	class LightPoint;

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

		void InitDirectionStructuredBuffer();

		void InitPointStructuredBuffer();

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

		/// <summary>
		/// 
		/// </summary>
		void RenderPointLight();

		//�`��I��
		void EndRender();

		/// <summary>
		/// 0�Ԗڂ̃f�B���N�V�������C�g��Ԃ�
		/// </summary>
		/// <returns></returns>
		LightDirection* GetMainLightDirection()
		{
			if (m_cDirLight.size() > 0)
			{
				return m_cDirLight[0];
			}
			else
			{
				return nullptr;
			}
		}
		//���C�g�̃p�����[�^�̍\����
		struct LightParam
		{
			CVector4 screen;						//�X�N���[���̃T�C�Y
			CVector3 eyePos = CVector3::Zero();		//���C���J�����̈ʒu
			int DLCount = 0;						//�f�B���N�V�������C�g�̐�
			int PLCount = 0;						//�|�C���g���C�g�̐��B
		};
	private:
		LightParam m_lightParam;						//���C�g�̏��
		ConstantBuffer m_lightParamCB;				//���C�g�p�����[�^�̒萔�o�b�t�@
		std::vector<LightDirection*> m_cDirLight;	//�f�B���N�V�������C�g�̃��X�g
		SDirectionLight m_sDirLights[MAX_DIRLIGHT];	//�f�B���N�V�������C�g�\���̂̃��X�g
		StructuredBuffer m_dirLightSB;				//�f�B���N�V�������C�g��StructuredBuffer
		std::vector<LightPoint*> m_cPointLight;
		SPointLight m_sPntLights[MAX_PNTLIGHT];
		StructuredBuffer m_pntLightSB;

		struct CameraParam
		{
			CMatrix projMat;
			CMatrix projInvMat;
			CMatrix viewRotMat;
			float camNear;
			float camFar;
			float screenWidth;
			float screenHeight;
		};
		ConstantBuffer m_cameraParamCB;
		Shader m_csPointLightCulling;
	};
}