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

private:
	ue::SkinModelRender* m_cannon = nullptr;
	ue::SkinModelRender* m_stand = nullptr;
	ue::MeshCollider m_meshCollider;
	ue::RigidBody m_rigidBody;
};