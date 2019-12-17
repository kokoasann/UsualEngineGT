#pragma once
#include "physics/BoxCollider.h"

namespace UsualEngine
{
	class CharacterBoxCollider:public GameObject
	{
	public:
		using SettingTagFunc = std::function<int(const char* tag)>;
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

		void Init(const wchar_t* path, Skeleton* skeleton, float scale, SettingTagFunc func = [](auto tag) {return enCollisionAttr_NonHitIK;});

		bool Start() override;
		void Update() override;

	private:
	
		std::vector<RelationBB> m_relationBBList;
		std::vector<BoxCollider> m_boxs;
		std::vector<RigidBody> m_rigids;
		bool m_isRigidBody = true;
	};
}