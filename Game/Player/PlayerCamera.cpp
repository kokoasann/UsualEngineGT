#include "stdafx.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "Cannon/Cannon.h"

PlayerCamera::PlayerCamera()
{
	m_camera = &ue::usualEngine()->GetMainCamera();
	m_p2cLen = 84.f;
	m_c2tLen = 6.f;
	m_p2c = { 0.f,0,-15 };
	m_p2c.Normalize();
	m_c2t = { 0.f,0,7 };
	m_c2t.Normalize();
	m_offsetY = 20;
}

void PlayerCamera::Release()
{
}

void PlayerCamera::OnDestroy()
{
}

void PlayerCamera::Init(Player* p)
{
	m_player = p;

#if _DEBUG
	std::function<void()> on = [&]()
	{
		m_isCameraOff = true;
	};
	std::function<void()> off = [&]()
	{
		m_isCameraOff = false;
	};
	ue::DebugSwitchAddRadio(ue::DebugSwitchNewSwitch('Z', VK_HOME, on, off));
#endif
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

		/*m_camera->SetPosition(campos);
		m_camera->SetTarget(camtar);
		m_camera->Update();*/
	}
	else
	{
		float yx = pad.GetRStickXF();
		float yy = pad.GetRStickYF();
		float speed = 5.f;
		ue::CQuaternion add;
		ue::CQuaternion r;

		add.SetRotationDeg(ue::CVector3::Up(), speed * yx);
		add.Multiply(m_cannonDir);

		auto buf = m_cannonDir;
		auto bufadd = add;

		ue::CVector3 axix;
		axix.Cross(m_cannonDir, ue::CVector3::AxisY());
		axix.Normalize();
		r.SetRotationDeg(axix, speed * yy);
		add.Multiply(r);
		add.Multiply(m_cannonDir);
		float rad = acos(ue::CVector3::Up().Dot(m_cannonDir));
		if (rad > ue::CMath::PI * 0.5f || rad < ue::CMath::PI * 0.025f)
		{
			m_cannonDir = buf;
			add = bufadd;
		}

		auto ofs = m_cannonDir;
		ofs.Cross(axix);

		campos = m_cannon->GetPos();
		campos += ofs*-100.f;
		camtar = campos + m_cannonDir;
		m_cannonDir.Normalize();
		m_cannon->SetTarget(m_cannonDir);
	}
	
#if _DEBUG
	if (m_isCameraOff)
	{
		m_camera->SetPosition(campos);
		m_camera->SetTarget(camtar);
		m_camera->Update();
	}
#else
	m_camera->SetPosition(campos);
	m_camera->SetTarget(camtar);
	m_camera->Update();
#endif
}

void PlayerCamera::NormalMode()
{
	if (m_cannon != nullptr)
	{
		m_cannon->SetActive(false);
		m_cannon->EndDirEdit();
	}
	m_cameraMode = CM_Normal;
}

void PlayerCamera::CannonMode(Cannon* cn)
{
	m_cannon = cn;
	cn->SetIKActive(true);
	cn->DirEdit();

	auto b = cn->GetDirBone();
	ue::CVector3 pos, sca;
	ue::CQuaternion rot;
	b->GetWorldMatrix().CalcMatrixDecompose(pos,rot,sca);
	//rot.SetRotation(b->GetWorldMatrix());
	m_cannonDir = ue::CVector3::AxisY();
	rot.Multiply(m_cannonDir);
	m_cameraMode = CM_Cannon;
}
