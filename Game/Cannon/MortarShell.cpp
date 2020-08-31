#include "stdafx.h"
#include "MortarShell.h"
#include "MortarShell_Explosion.h"

struct SweepResultShell :public btCollisionWorld::ConvexResultCallback
{
public:
	bool isHit = false;
	int hitTag = 0;
	// ConvexResultCallback を介して継承されました
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) override
	{
		int ind = convexResult.m_hitCollisionObject->getUserIndex();
		if (ind & CUI_Cannon || ind & ue::enCOllisionAttr_IK)
			return 0.f;
		hitTag = ind;
		isHit = true;
		return 1.f;
	}
};

MortarShell::~MortarShell()
{
	ue::DeleteGO(m_model);
}

void MortarShell::OnDestroy()
{
	ue::DeleteGO(m_model);
	//ue::Physics().RemoveRigidBody(m_rigidBody);
	//m_rigidBody.Release();
	m_collider.Release();
	
}

void MortarShell::Init(ue::CVector3 pos, ue::CVector3 dir)
{
	m_pos = pos;
	m_dir = dir*m_power;
	m_oldDir = dir;
	m_model = ue::NewGO<ue::SkinModelRender>(0);
	m_model->Init(L"Assets/model/MortarShell.cmo", 0, 0, ue::enFbxUpAxisY);
	m_model->SetSca(ue::CVector3::One() * 2.f);
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

	m_collider.Create(1);
	ue::RigidBodyInfo info;
	info.collider = &m_collider;
	info.mass = 0.f;
	info.pos = pos;
	info.rot = rot;
	//m_rigidBody.Create(info);
	//ue::Physics().AddRigidBody(m_rigidBody);
}

bool MortarShell::Start()
{
	return true;
}

void MortarShell::Update()
{
	float dtime = ue::gameTime()->GetDeltaTime();
	ue::CVector3 npos = m_pos + m_dir * dtime;

	if (npos.y < -500.f)
	{
		auto thisptr = this;
		ue::DeleteGO(thisptr);
		return;
	}
	
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin({ m_pos.x,m_pos.y,m_pos.z });
	end.setOrigin({ npos.x,npos.y,npos.z });
	SweepResultShell res;
	ue::Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, res);
	if (!res.isHit)
	{
		m_pos = npos;
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
		m_dir.y -= m_gravity * dtime;
	}
	else
	{
		auto explosion = ue::NewGO<MortarShell_Explosion>(0);
		explosion->SetPos(m_pos);

		auto thisptr = this;
		ue::DeleteGO(thisptr);
	}
}