#pragma once

#include "physics/SphereCollider.h"

namespace UsualEngine
{
	class IK
	{
	public:
		
		enum UseIK
		{
			enUSE_CCD,
		};
		IK(Bone* effector, Bone* end) :
			m_effectorBone(effector),
			m_endBone(end)
		{

		}
		IK();
		~IK(){}

		void Update(CMatrix& worldMat);

		void EXE_CCD(CMatrix& worldMat);

		void SetEffectorBone(Bone* bone)
		{
			m_effectorBone = bone;
		}
		void SetEndBone(Bone* bone)
		{
			m_endBone = bone;
		}
	private:
		Bone* m_effectorBone=0;
		Bone* m_endBone=0;
		int m_usingIK = enUSE_CCD;

		SphereCollider m_collider;

	};

}