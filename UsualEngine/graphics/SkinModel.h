#pragma once

#include "Skeleton.h"
#include "SkinModelEffect.h"

namespace UsualEngine
{
	/*!
	*@brief	FBX�̏�����B
	*/
	enum EnFbxUpAxis {
		enFbxUpAxisY,		//Y-up
		enFbxUpAxisZ,		//Z-up
	};
	/*!
	*@brief	�X�L�����f���N���X�B
	*/
	class SkinModel
	{
	public:
		//���b�V�������������Ƃ��̃R�[���o�b�N�֐��B
		using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
		//�}�e���A���̃R�[���o�b�N�֐��B
		using OnFindMaterial = std::function<void(ModelEffect*)>;
		/*!
		*@brief	�f�X�g���N�^�B
		*/
		~SkinModel();
		/// <summary>
		/// �J���B
		/// </summary>
		void Release();

		/*!
		*@brief	�������B
		*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
		*@param[in] enFbxUpAxis		fbx�̏㎲�B�f�t�H���g��enFbxUpAxisZ�B
		*/
		void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);

		void InitInstancing(unsigned int maxInstance);
		/*!
		*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
		*@param[in]	position	���f���̍��W�B
		*@param[in]	rotation	���f���̉�]�B
		*@param[in]	scale		���f���̊g�嗦�B
		*/
		void UpdateWorldMatrix(const CVector3& position, const CQuaternion& rotation, const CVector3& scale);
		/// <summary>
		/// ���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
		/// </summary>
		/// <param name="wmat">���[���h�}�g���N�X</param>
		void UpdateWorldMatrix(const CMatrix& wmat);
		/*!
		*@brief	�{�[���������B
		*@param[in]		boneName	�{�[���̖��O�B
		*@return	���������{�[���B������Ȃ������ꍇ��nullptr��Ԃ��܂��B
		*/
		Bone* FindBone(const wchar_t* boneName)
		{
			int boneId = m_skeleton.FindBoneID(boneName);
			return m_skeleton.GetBone(boneId);
		}
		/*!
		*@brief	���f����`��B
		*@param[in]	viewMatrix		�J�����s��B
		*  ���[���h���W�n��3D���f�����J�������W�n�ɕϊ�����s��ł��B
		*@param[in]	projMatrix		�v���W�F�N�V�����s��B
		*  �J�������W�n��3D���f�����X�N���[�����W�n�ɕϊ�����s��ł��B
		*/
		void Draw(CMatrix viewMatrix, CMatrix projMatrix);
		/*!
		*@brief	�X�P���g���̎擾�B
		*/
		Skeleton& GetSkeleton()
		{
			return m_skeleton;
		}
		/*!
		*@brief	���b�V������������B
		*@param[in] onFindMesh		���b�V�������������Ƃ��̃R�[���o�b�N�֐�
		*/
		void FindMesh(OnFindMesh onFindMesh) const
		{
			for (auto& modelMeshs : m_modelDx->meshes) {
				for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
					onFindMesh(mesh);
				}
			}
		}
		
		/// <summary>
		/// �}�e���A������������
		/// </summary>
		/// <param name="func">�}�e���A���̃R�[���o�b�N�֐�</param>
		void FindMaterial(OnFindMaterial func)
		{
			FindMesh([&](auto& mesh)
			{
					ModelEffect* eff = reinterpret_cast<ModelEffect*>(mesh->effect.get());
					func(eff);
			});
		}
		/*!
		*@brief	SRV�̃��W�X�^�ԍ��B
		*/
		enum EnSkinModelSRVReg {
			enSkinModelSRVReg_DiffuseTexture = 0,		//!<�f�B�t���[�Y�e�N�X�`���B
			enSkinModelSRVReg_BoneMatrix,				//!<�{�[���s��B
		};

		/// <summary>
		/// �V���h�E���o���܂����H
		/// </summary>
		/// <param name="b">true or false</param>
		void SetIsShadowCaster(bool b)
		{
			m_isShadowCaster = b;
		}
		/// <summary>
		/// �V���h�E�o���H
		/// </summary>
		/// <returns></returns>
		bool IsShadowCaster()
		{
			return m_isShadowCaster;
		}

		/// <summary>
		/// �V���h�E���󂯎��܂����H
		/// </summary>
		/// <param name="b">true or false</param>
		void SetIsShadowReciever(bool b)
		{
			m_isShadowReciever = b;
		}
		/// <summary>
		/// �V���h�E�󂯂�H
		/// </summary>
		/// <returns></returns>
		bool IsShadowReciever()
		{
			return m_isShadowReciever;
		}

		/// <summary>
		/// ���f���̏㎲��Ԃ�
		/// </summary>
		/// <returns></returns>
		EnFbxUpAxis GetFbxUpAxis()
		{
			return m_enFbxUpAxis;
		}

		void SetSpecularMap(const wchar_t* path);

		void SetInstanceData(unsigned int num,const CMatrix& mat)
		{
			m_instancingData[num] = mat;
		}
	private:
		/*!
		*@brief	�T���v���X�e�[�g�̏������B
		*/
		void InitSamplerState();
		/*!
		*@brief	�萔�o�b�t�@�̍쐬�B
		*/
		void InitConstantBuffer();
		/*!
		*@brief	�X�P���g���̏������B
		*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
		*/
		void InitSkeleton(const wchar_t* filePath);

	private:
		//�萔�o�b�t�@�B
		struct SVSConstantBuffer {
			CMatrix mWorld;
			CMatrix mView;
			CMatrix mProj;
			CVector3 camDir;
			int isShadowReciever;
			int isUseSpecularMap = 0;
		};
		EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBX�̏�����B
		ID3D11Buffer* m_cb = nullptr;					//!<�萔�o�b�t�@�B
		Skeleton			m_skeleton;						//!<�X�P���g���B
		CMatrix				m_worldMatrix;					//!<���[���h�s��B
		DirectX::Model* m_modelDx;						//!<DirectXTK���񋟂��郂�f���N���X�B
		ID3D11SamplerState* m_samplerState = nullptr;		//!<�T���v���X�e�[�g�B

		ID3D11ShaderResourceView* m_specularMap = nullptr;

		bool m_isShadowCaster = false;					//�e�𗎂Ƃ�
		bool m_isShadowReciever = true;				//�e��������

		bool m_isInstancing = false;
		unsigned int m_maxInstance = 0;
		unsigned int m_numInstance = 0;
		CMatrix* m_instancingData = nullptr;
		StructuredBuffer m_instancingDataSB;
	};

}