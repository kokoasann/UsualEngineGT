#pragma once

#include "physics/SphereCollider.h"
#include "Physics/RigidBody.h"

namespace UsualEngine
{
	class IK
	{
	public:
		/// <summary>
		/// IK�̎�ށB
		/// </summary>
		enum UseIK
		{
			enUSE_CCD,
		};

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="effector">�G�t�F�N�^�{�[��</param>
		/// <param name="end">�G���h�{�[��(�G�t�F�N�^�{�[���̐e�����ǂ��Ă����ƃG���h�{�[���ɂȂ�悤�Ȋ֌W�łȂ���΂Ȃ�Ȃ��B)</param>
		/// <param name="radius">IK�Ɏg�p����X�t�B�A�R���C�_�[�̔��a</param>
		IK(Bone* effector, Bone* end, float radius);
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~IK();

		/// <summary>
		/// �X�V�B
		/// </summary>
		/// <param name="worldMat"></param>
		void Update(CMatrix& worldMat);

	private:
		/// <summary>
		/// CCDIK�̎��s
		/// </summary>
		/// <param name="worldMat"></param>
		void EXE_CCD(CMatrix& worldMat);

		/// <summary>
		/// ���W�b�h�{�f�B�̏������B
		/// </summary>
		void InitRigidBody();

		/// <summary>
		/// ���W�b�h�{�f�B�̍X�V
		/// </summary>
		/// <param name="pos"></param>
		void UpdateRigidBody(CVector3 pos);

	public:
		/// <summary>
		/// IK�̐�[�̃{�[��
		/// </summary>
		/// <param name="bone"></param>
		void SetEffectorBone(Bone* bone)
		{
			m_effectorBone = bone;
		}
		/// <summary>
		/// IK�̏I���̃{�[��
		/// </summary>
		/// <param name="bone"></param>
		void SetEndBone(Bone* bone)
		{
			m_endBone = bone;
		}
	private:
		Bone* m_effectorBone=0;			//�G�t�F�N�^�{�[��
		Bone* m_endBone=0;					//�G���h�{�[��
		float m_radius = 0.f;					//�R���C�_�[�̔��a�B
		int m_usingIK = enUSE_CCD;		//�g��IK�̎�ށB

		SphereCollider m_collider;			//�R���C�_�[
		RigidBody m_rigidBody;				//���W�b�h�{�f�B
	};

}