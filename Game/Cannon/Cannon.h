#pragma once
#include "physics/MeshCollider.h"

class Cannon :public ue::GameObject
{
public:
	Cannon(){}
	~Cannon(){}
	void OnDestroy() override;
	void Init(const ue::SkinModel& cannonMesh,const ue::CVector3& pos,const ue::CQuaternion& rot);
	bool Start() override;
	void Update() override;

	ue::Skeleton& GetSkeleton()
	{
		return m_cannon->GetSkinModel().GetSkeleton();
	}
	void SetIKActive(bool b)
	{
		m_cannon->GetAnimation().SetIKActive(b);
	}
	const ue::CVector3& GetPos() const
	{
		return m_pos;
	}
	const ue::CQuaternion& GetRot() const
	{
		return m_rot;
	}
	void SetDir(const ue::CVector3& dir)
	{
		m_dir = dir;
	}
	void SetTarget(const ue::CVector3& dir)
	{
		auto nonY = dir;
		nonY.y = 0;
		nonY.Normalize();
		float r = atan2(nonY.x, nonY.z);
		ue::CQuaternion rot;
		rot.SetRotation(ue::CVector3::Up(), r);
		m_cannon->SetRot(rot);
		m_stand->SetRot(rot);
		m_rot = rot;
		m_cannon->GetAnimation().SetIKNextTarget(m_dirBone->GetParent()->GetWorldMatrix().GetTranslation()+(dir*80.f), m_dirBone);
		
	}
	ue::Bone* GetDirBone()
	{
		return m_dirBone;
	}
private:
	ue::SkinModelRender* m_cannon = nullptr;
	ue::SkinModelRender* m_stand = nullptr;
	ue::MeshCollider m_meshCollider;
	ue::RigidBody m_rigidBody;

	ue::CVector3 m_dir = ue::CVector3::Zero();
	ue::CVector3 m_pos = ue::CVector3::Zero();
	ue::CQuaternion m_rot = ue::CQuaternion::Identity();
	ue::Bone* m_dirBone = nullptr;
	ue::CQuaternion m_collisionRot = ue::CQuaternion::Identity();
};