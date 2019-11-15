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
		/// リジッドボディの初期化。
		/// </summary>
		void InitRigidBody();
		/// <summary>
		/// リジッドボディの更新
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
		RigidBody m_rigidBody;				//リジッドボディ

		CVector3 m_target = CVector3::Zero();
		CVector3 m_move = CVector3::Zero();
		bool m_isHit = false;			
	};

}