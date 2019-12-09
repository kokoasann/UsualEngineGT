#include "stdafx.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "Cannon/Cannon.h"

PlayerCamera::PlayerCamera()
{
	m_camera = &ue::usualEngine()->GetMainCamera();
	m_p2cLen = 840.f;
	m_c2tLen = 60.f;
	m_p2c = { 0.f,0,-15 };
	m_p2c.Normalize();
	m_c2t = { 0.f,0,7 };
	m_c2t.Normalize();
	m_offsetY = 200;
}

void PlayerCamera::OnDestroy()
{
}

void PlayerCamera::Init(Player* p)
{
	m_player = p;
}

bool PlayerCamera::Start()
{
	return true;
}

void PlayerCamera::Update()
{
	ue::CVector3 campos, camtar;
	auto pad = ue::usualEngine()->GetPad(0);
	if (m_cameraMode == CM_Normal)
	{
		auto ppos = m_player->GetPos();

		float yx = pad.GetRStickXF();
		float yy = pad.GetRStickYF();
		float speed = 5.f;
		ue::CQuaternion add;
		ue::CQuaternion r = ue::CQuaternion::Identity();

		add.SetRotationDeg(ue::CVector3::Up(), speed * yx);
		//camrot.Multiply(add);
		add.Multiply(m_p2c);
		add.Multiply(m_c2t);

		ue::CVector3 axix = ue::CVector3::Zero();
		axix.Cross(m_c2t, ue::CVector3::AxisY());
		axix.Normalize();
		r.SetRotationDeg(axix, speed * yy);
		add.Multiply(r);
		add.Multiply(m_p2c);
		add.Multiply(m_c2t);

		campos = m_p2c * m_p2cLen + ppos;
		camtar = m_c2t * m_c2tLen + campos;

		campos.y += m_offsetY;
		camtar.y += m_offsetY;
	}
	else
	{
		float yx = pad.GetRStickXF();
		float yy = pad.GetRStickYF();
		float speed = 5.f;
		ue::CQuaternion add;
		ue::CQuaternion r = ue::CQuaternion::Identity();

		add.SetRotationDeg(ue::CVector3::Up(), speed * yx);
		add.Multiply(m_cannonDir);

		auto buf = m_cannonDir;

		ue::CVector3 axix = ue::CVector3::Zero();
		axix.Cross(m_c2t, ue::CVector3::AxisY());
		axix.Normalize();
		r.SetRotationDeg(axix, speed * yy);
		add.Multiply(r);
		add.Multiply(m_cannonDir);
		float rad = acos(ue::CVector3::Up().Dot(m_cannonDir));
		if (rad > ue::CMath::PI * 0.5f || rad < ue::CMath::PI * 0.025f)
		{
			m_cannonDir = buf;
		}
		campos = m_cannon->GetPos();
		campos.y += 50.f;
		camtar = m_cannonDir;
	}
	
#if 1
	m_camera->SetPosition(campos);
	m_camera->SetTarget(camtar);
	m_camera->Update();
#endif
}

void PlayerCamera::CannonMode(Cannon* cn)
{
	m_cannon = cn;
	cn->SetIKActive(true);

	m_cameraMode = CM_Cannon;
}
