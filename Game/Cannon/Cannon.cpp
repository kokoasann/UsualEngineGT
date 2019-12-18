#include "stdafx.h"
#include "Cannon.h"
#include "MortarShell.h"


void Cannon::Release()
{
	ue::DeleteGO(m_cannon);
	ue::DeleteGO(m_stand);
}

void Cannon::OnDestroy()
{
	Release();
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
	//body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
	body->setUserIndex(ue::enCollisionAttr_NonHitIK | CollisionUserIndex::CUI_Cannon);
	body->setUserPointer(this);
	//body->setActivationState(DISABLE_SIMULATION);
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

	m_dirBone = m_cannon->GetSkinModel().GetSkeleton().GetBone(2);
	m_cannon->SetingIK(m_dirBone, m_dirBone->GetParent(), 10,false);
	m_cannon->GetAnimation().SetIKActive(false);
	m_pos = pos;
	m_rot = rot;
	//m_collisionRot = rot;

	m_rot.Multiply(m_dir);
	m_isFirst = 0;
}

bool Cannon::Start()
{
	return true;
}

void Cannon::Update()
{
	if (!m_isDirEdit && !m_isFirst>=2)
		return;
	Update_RigidBody();
	if(m_isFirst<=3)
		m_isFirst++;
}

void Cannon::Update_RigidBody()
{
	ue::CVector3 pos, sca;
	ue::CQuaternion rot, ofsrot;
	const auto& mat = m_dirBone->GetWorldMatrix();
	mat.CalcMatrixDecompose(pos, rot, sca);
	/*ue::CVector3 axi = { mat.m[2][0],mat.m[2][1],mat.m[2][2] };
	axi.Normalize();*/
	ofsrot.SetRotation({ 1,0,0 }, ue::CMath::PI / -2.f);
	rot.Multiply(ofsrot);
	ofsrot.SetRotation({ 0,0,1 }, ue::CMath::PI);
	rot.Multiply(ofsrot);

	auto rigid = m_rigidBody.GetBody();
	auto & trans = rigid->getWorldTransform();
	trans.setRotation({ rot.x,rot.y,rot.z,rot.w });
	auto p = m_dirBone->GetParent()->GetWorldMatrix().GetTranslation();
	trans.setOrigin({ p.x ,p.y ,p.z });
}

void Cannon::Fire()
{
	auto shell = ue::NewGO<MortarShell>(0);
	shell->Init(m_dirBone->GetParent()->GetWorldMatrix().GetTranslation(), m_dir);
}
