#pragma once

#include "physics/SphereCollider.h"
#include "Physics/RigidBody.h"

namespace UsualEngine
{
	class IK
	{
	public:
		
		enum UseIK
		{
			enUSE_CCD,
		};
		IK(Bone* effector, Bone* end, float radius);
		~IK();

		void UpdateTarget(const CMatrix& worldMat);
		void Update(const CMatrix& worldMat);

		void EXE_CCD(const CMatrix& worldMat);

		/// <summary>
		/// ���W�b�h�{�f�B�̏������B
		/// </summary>
		void InitRigidBody();
		/// <summary>
		/// ���W�b�h�{�f�B�̍X�V
		/// </summary>
		/// <param name="pos"></param>
		void UpdateRigidBody(CVector3 pos);

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
		Bone* m_effectorBone=0;
		Bone* m_endBone=0;
		float m_radius = 0.f;
		int m_usingIK = enUSE_CCD;

		SphereCollider m_collider;
		RigidBody m_rigidBody;				//���W�b�h�{�f�B

		CVector3 m_target = CVector3::Zero();
		CVector3 m_move = CVector3::Zero();
		bool m_isHit = false;			
	};

}