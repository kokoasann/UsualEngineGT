#include "stdafx.h"
#include "MortarShell.h"

MortarShell::~MortarShell()
{
	ue::DeleteGO(m_model);
}

void MortarShell::OnDestroy()
{
	ue::DeleteGO(m_model);
	ue::Physics().RemoveRigidBody(m_rigidBody);
	m_rigidBody.Release();
	m_collider.Release();
	
}

void MortarShell::Init(ue::CVector3 pos, ue::CVector3 dir)
{
	m_pos = pos;
	m_dir = dir*m_power;
	m_oldDir = dir;
	m_model = ue::NewGO<ue::SkinModelRender>(0);
	m_model->Init(L"Assets/model/MortarShell.cmo", 0, 0, ue::enFbxUpAxisY);
	m_model->SetSca(ue::CVector3::One() * 20.f);
	m_model->SetPos(pos);

	auto nonY = dir;
	nonY.y = 0.0f;
	nonY.Normalize();

	ue::CQuaternion rot;
	ue::CVector3 ax;
	ax.Cross(ue::CVector3::AxisZ(), dir);
	ax.Normalize();
	float t = ue::CVector3::AxisZ().Dot(dir);
	rot.SetRotation(ax, acosf(t));

	m_model->SetRot(rot);
	m_rot = rot;

	m_collider.Create(10, 15);
	ue::RigidBodyInfo info;
	info.collider = &m_collider;
	info.mass = 0.f;
	info.pos = pos;
	info.rot = rot;
	m_rigidBody.Create(info);
	ue::Physics().AddRigidBody(m_rigidBody);
}

bool MortarShell::Start()
{
	return true;
}

void MortarShell::Update()
{
	float dtime = ue::gameTime()->GetDeltaTime();
	m_pos += m_dir*dtime;
	m_model->SetPos(m_pos);		//ポジションの更新。

	auto dir = m_dir;			//回転の更新。
	dir.Normalize();
	ue::CQuaternion rot;
	ue::CVector3 ax;
	ax.Cross(ue::CVector3::AxisZ(), dir);
	ax.Normalize();
	float t = ue::CVector3::AxisZ().Dot(dir);
	rot.SetRotation(ax, acosf(fabsf(t) < 1.f ? t : t / fabsf(t)));
	m_model->SetRot(rot);
	
	//m_oldDir = dir;
	float len = m_dir.Length();				//加速の減衰。
	//m_dir.x -= m_dir.x * len * dtime;
	//m_dir.z -= m_dir.z * len * dtime;
	m_dir.y -= m_gravity * dtime;// -m_dir.y * len * dtime;

	auto tr = m_rigidBody.GetBody()->getWorldTransform();
	tr.setOrigin({ m_pos.x,m_pos.y ,m_pos.z });
	tr.setRotation({ rot.x,rot.y ,rot.z });

	if (m_pos.y <= 0)		//(仮)
	{
		auto thisptr = this;
		ue::DeleteGO(thisptr);
	}
}