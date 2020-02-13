#pragma once

#include "physics/SphereCollider.h"
#include "Physics/RigidBody.h"

namespace UsualEngine
{
	class IK
	{
	public:
		enum IKMode
		{
			enMode_Normal,
			enMode_Foot,		//footIK
		};
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
		IK(Bone* effector, Bone* end, float radius, bool isOnRigitBody = false , const CVector3& pos = CVector3::Zero());
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~IK();

		/// <summary>
		/// IK�̃^�[�Q�b�g�ƂȂ�ꏊ���X�V����
		/// </summary>
		/// <param name="worldMat">���f���̃��[���h�}�g���N�X</param>
		void UpdateTarget(const CMatrix& worldMat);
		void UpdateTarget_Normal(const CMatrix& worldMat);
		void UpdateTarget_Foot(const CMatrix& worldMat);
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
		/// <param name="mode"></param>
		void SetIKMode(IKMode mode)
		{
			m_ikMode = mode;
		}

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
		Bone* GetEffectorBone() const
		{
			return m_effectorBone;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="bone"></param>
		void SetEndBone(Bone* bone)
		{
			m_endBone = bone;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Bone* GetEndBone() const
		{
			return m_endBone;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const CVector3& GetTarget() const
		{
			return m_target;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const CVector3& GetOldTarget() const
		{
			return m_oldTarget;
		}
		/// <summary>
		/// ���C�����Q�b�g
		/// </summary>
		/// <returns></returns>
		float GetRubbing() const
		{
			return m_rubbing;
		}
		/// <summary>
		/// ���C�����Z�b�g
		/// </summary>
		/// <param name="f">1~0������</param>
		void SetRubbing(float f)
		{
			m_rubbing = f;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		void SetOffset(const CVector3& v)
		{
			m_offset = v;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="f"></param>
		void SetSpeed(float f)
		{
			m_speed = f;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="b"></param>
		void SetIsActive(bool b)
		{
			m_isActive = b;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsActive() const
		{
			return m_isActive;
		}
		void SetNextTarget(const CVector3& tar)
		{
			m_nextTarget = tar;
			m_isSetNextTarget = true;
		}

		void SetGravity(const float grav)
		{
			m_gravity = grav;
		}

		const CVector3& GetAnimMove() const
		{
			return m_animMove;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const CVector3& GetMove() const
		{
			return m_move;
		}

		bool IsHit() const
		{
			return m_isHit;
		}

		btCollisionObject* GetHitCollision() const
		{
			return m_hitCollision;
		}

	private:
		bool m_isFirst = true;							//���߂Ă��H
		Bone* m_effectorBone=0;							//�G�t�F�N�^�{�[��
		Bone* m_endBone=0;								//�G���h�{�[��
		float m_radius = 0.f;							//�R���C�_�[�̔��a�B
		UseIK m_usingIK = enUSE_CCD;					//�g�p����IK�̎�ށB
		IKMode m_ikMode = enMode_Normal;				//IK�̃��[�h

		SphereCollider m_collider;						//�R���C�_�[
		RigidBody m_rigidBody;							//���W�b�h�{�f�B

		CVector3 m_offset = CVector3::Zero();			
		CVector3 m_target = CVector3::Zero();			//IK�̃^�[�Q�b�g
		CVector3 m_oldTarget = CVector3::Zero();		//old IK�̃^�[�Q�b�g
		CVector3 m_rubTarget = CVector3::Zero();		//�C������̃|�W�V�����B
		CVector3 m_move = CVector3::Zero();				//�ړ��x�N�g��
		CVector3 m_animMove = CVector3::Zero();
		bool m_isUseLocalTarget = 1;
		float m_speed = 1.f;							//���̃^�[�Q�b�g�Ɉړ�����Ƃ��̑��x(%)�B1�Ȃ�1�t���[���ňړ�����B0�Ȃ瓮���Ȃ��B

		bool m_isHit = false;							//���������H
		float m_rubbing = 1.0f;							//���C���A�����l��100%�@���邱�Ƃ͂Ȃ��B

		bool m_isActive = true;							//�����H
		CVector3 m_nextTarget = CVector3::Zero();		//�O������ݒ肳�ꂽ���̃^�[�Q�b�g
		bool m_isSetNextTarget = false;					//���̃^�[�Q�b�g���Z�b�g���ꂽ�H
		
		bool m_isUseRigidBody = true;					//���W�b�h�{�f�B�[���g���H

		float m_gravity = 0.0f;							// �d�́B
		float m_timer = 0.0f;							//�d�͗p�̃^�C�}�[
		bool m_isNextGravity = false;					
		float m_gravitPow = 0.0f;							
		CVector3 m_oldNewTarget = CVector3::Zero();		

		btCollisionObject* m_hitCollision = nullptr;
	};

}