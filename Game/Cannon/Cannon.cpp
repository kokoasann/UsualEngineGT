#include "stdafx.h"
#include "Cannon.h"

void Cannon::OnDestroy()
{
	ue::DeleteGO(m_cannon);
	ue::DeleteGO(m_stand);
}

void Cannon::Init(const ue::SkinModel& cannonMesh, const ue::CVector3& pos, const ue::CQuaternion& rot)
{
	ue::CVector3 sca = ue::CVector3::One() * 20.f;
	ue::CMatrix scam;
	scam.MakeScaling(sca);

	ue::CMatrix rotm;
	rotm.MakeRotationY(ue::CMath::DegToRad(-90.f));
	scam *= rotm;

	m_meshCollider.CreateFromSkinModel(cannonMesh, &scam);
	ue::RigidBodyInfo info;
	info.collider = &m_meshCollider;
	info.mass = 10.f;
	info.pos = pos;
	info.rot = rot;
	m_rigidBody.Create(info);
	auto* body = m_rigidBody.GetBody();
	body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
	body->setUserIndex(enCollisionAttr_NonHitIK);
	ue::Physics().AddRigidBody(m_rigidBody);

	m_cannon = ue::NewGO<ue::SkinModelRender>(0);
	m_cannon->Init(L"Assets/model/Cannon.cmo", 0, 0, ue::enFbxUpAxisY);
	m_cannon->SetPos(pos);
	m_cannon->SetRot(rot);
	m_cannon->SetSca(sca);
	m_stand = ue::NewGO<ue::SkinModelRender>(0);
	m_stand->Init(L"Assets/model/Stand.cmo", 0, 0, ue::enFbxUpAxisY);
	m_stand->SetPos(pos);
	m_stand->SetRot(rot);
	m_stand->SetSca(sca);
}

bool Cannon::Start()
{
	return true;
}

void Cannon::Update()
{
}
