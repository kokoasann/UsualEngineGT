#pragma once

#include "physics/SphereCollider.h"
#include "Physics/RigidBody.h"

namespace UsualEngine
{
	class IK
	{
	public:
		/// <summary>
		/// �g�p����IK�̎�ށB
		/// </summary>
		enum UseIK
		{
			enUSE_CCD,
		};
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="effector">�G�t�F�N�^�{�[��</param>
		/// <param name="end">�G���h�{�[��</param>
		/// <param name="radius">�R���C�_�[�̔��a</param>
		IK(Bone* effector, Bone* end, float radius,const CVector3& pos = CVector3::Zero());
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~IK();

		/// <summary>
		/// IK�̃^�[�Q�b�g�ƂȂ�ꏊ���X�V����
		/// </summary>
		/// <param name="worldMat">���f���̃��[���h�}�g���N�X</param>
		void UpdateTarget(const CMatrix& worldMat);
		/// <summary>
		/// IK�̍X�V�B
		/// </summary>
		/// <param name="worldMat">���f���̃��[���h�}�g���N�X</param>
		void Update(const CMatrix& worldMat);

		/// <summary>
		/// CCD_IK�����s�B
		/// </summary>
		/// <param name="worldMat">���f���̃��[���h�}�g���N�X</param>
		void EXE_CCD(const CMatrix& worldMat);

		/// <summary>
		/// ���W�b�h�{�f�B�̏������B
		/// </summary>
		void InitRigidBody(const CVector3& pos);
		/// <summary>
		/// ���W�b�h�{�f�B�̍X�V
		/// </summary>
		/// <param name="pos"></param>
		void UpdateRigidBody(const CVector3& pos);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="bone"></param>
		void SetEffectorBone(Bone* bone)
		{
			m_effectorBone = bone;
		}
		/// <summary>
		/// 
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
		int m_usingIK = enUSE_CCD;		//�g�p����IK�̎�ށB

		SphereCollider m_collider;			//�R���C�_�[
		RigidBody m_rigidBody;				//���W�b�h�{�f�B

		CVector3 m_target = CVector3::Zero();	//IK�̃^�[�Q�b�g
		CVector3 m_move = CVector3::Zero();	//�ړ��x�N�g��
		bool m_isHit = false;			//���������H
	};

}