#include "stdafx.h"
#include "PlayerCamera.h"
#include "Player.h"

PlayerCamera::PlayerCamera()
{
	m_camera = &ue::usualEngine()->GetMainCamera();
	m_p2cLen = 180.f;
	m_c2tLen = 60.f;
	m_p2c = { 1,0,-15 };
	m_p2c.Normalize();
	m_c2t = { 1.f,0,7 };
	m_c2t.Normalize();
	m_offsetY = 100;
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
	auto ppos = m_player->GetPos();

	auto pad = ue::usualEngine()->GetPad(0);
	
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

	auto campos = m_p2c * m_p2cLen + ppos;
	auto camtar = m_c2t * m_c2tLen + campos;

	campos.y += m_offsetY;
	camtar.y += m_offsetY;
	
#if 1
	m_camera->SetPosition(campos);
	m_camera->SetTarget(camtar);
	m_camera->Update();
#endif
}
