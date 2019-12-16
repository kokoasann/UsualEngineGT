#pragma once
#include "physics/BoxCollider.h"

namespace UsualEngine
{
	class CharacterBoxCollider:public GameObject
	{
	public:
		struct RelationBB
		{
			Bone* bone;
			BoxCollider box;
			RigidBody* rigid;
			CMatrix offsetmat = CMatrix::Identity();
		};
		CharacterBoxCollider() {};
		~CharacterBoxCollider() {};

		virtual void Release() override;
		virtual void OnDestroy() override;

		void Init(const wchar_t* path,Skeleton* skeleton, float scale,bool isRigidBody=true);

		bool Start() override;
		void Update() override;

	private:
	
		std::vector<RelationBB> m_relationBBList;
		std::vector<BoxCollider> m_boxs;
		std::vector<RigidBody> m_rigids;
		bool m_isRigidBody = true;
	};
}