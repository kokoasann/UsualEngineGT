#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	m_anim[PA_idol].Load(L"Assets/model/Player/player_idol.tka");
	m_anim[PA_idol].SetLoopFlag(true);
	m_anim[PA_walk].Load(L"Assets/model/Player/player_walk.tka");
	m_anim[PA_walk].SetLoopFlag(true);
	m_anim[PA_walkFast].Load(L"Assets/model/Player/player_walkFast.tka");
	m_anim[PA_walkFast].SetLoopFlag(true);
	m_anim[PA_dush].Load(L"Assets/model/Player/player_dush.tka");
	m_anim[PA_dush].SetLoopFlag(true);

	ue::SkinModelRender* model = ue::NewGO<ue::SkinModelRender>(0);
	model->Init(L"Assets/model/Player.cmo",m_anim,3,ue::enFbxUpAxisY);
	model->SetPos({ 0,100,0 });
	model->SetSca(ue::CVector3::One()*20.f);
	model->SetIsShadowCaster(true);
	model->SetIsShadowReciever(true);
	m_camera.Init(this);
	m_chara.Init(model, 20, 40, {0,-2.f,0});
	static ue::Bone* fL = m_chara.FindBone(L"Bone_L.003", Character::BK_FootL, true, 3, 12.5f);
	static ue::Bone* fR = m_chara.FindBone(L"Bone_R.003", Character::BK_FootR, true, 3, 12.5f);
	//m_chara.FindBone(L"Bone.003_L.003", Character::BK_HandL, true, 3, 1);
	//m_chara.FindBone(L"Bone.003_R.003", Character::BK_HandR, true, 3, 1);
	//m_chara.FindBone(L"Bone_L.005", Character::BK_None, true, 2, 4);
	//m_chara.FindBone(L"Bone_R.005", Character::BK_None, true, 2, 4);

	m_chara.SetMoveFunc([&](auto & move) {return;	});
	m_chara.SetRotateFunc([&](auto & rote) {return; });

	model->GetAnimation().Play(0);
	m_gmList[0] = &m_camera;
	m_gmList[1] = &m_chara;

	m_pad = &ue::GamePad(0);
}

Player::~Player()
{
}

bool Player::Start()
{
	return true;
}

void Player::Update()
{
	float x = m_pad->GetLStickXF();
	float y = m_pad->GetLStickYF();

	//x = m_pad->IsPress(ue::enButtonA);

	if (fabsf(y))
	{
		int a = 1 + 2;
	}
	if (fabsf(x) > 0.01f || fabsf(y) > 0.01f)
	{
		if (!m_isWalk)
		{
			
			//m_chara.SetMove(move);

			m_chara.PlayAnim(2, 1, Character::AM_Move);
			m_isWalk = true;
			m_chara.SetAllIKRub(1.0f);
			
		}
		
		auto cam = m_camera.GetCamera();
		auto f = cam->GetForward() * y;
		auto r = cam->GetRight() * x;
		auto move = f + r;
		move.Normalize();
		m_chara.SetIKOffset(move * 500.f * ue::gameTime()->GetDeltaTime());
		move *= 500;
		m_chara.SetMove(move);
		
		if (move.Length() > 0.1f)
		{
			move.y = 0;
			move.Normalize();
			float f = move.Dot(ue::CVector3::AxisZ());
			char st[255] = { 0 };
			sprintf_s(st, "DIR: x:%f y:%f z:%f\n", m_dir.x, m_dir.y, m_dir.z);
			//OutputDebugStringA(st);
			sprintf_s(st, "MOV: x:%f y:%f z:%f\n", move.x, move.y, move.z);
			//OutputDebugStringA(st);

			
			f = max(f, -1.f);
			f = min(f, 1.f);
			float rad = acosf(f);
			rad = atan2(move.x,move.z );
			ue::CQuaternion add;
			sprintf_s(st, "dg: %f\n", ue::CMath::RadToDeg(rad));
			//OutputDebugStringA(st);
			add.SetRotation(ue::CVector3::AxisY(), rad);
			m_dir = move;
			m_chara.SetRotation(add);

		}
	}
	else if (m_isWalk)
	{
		m_chara.PlayAnim(0, 1, Character::AM_None);
		m_isWalk = false;
		m_chara.SetAllIKRub(0.0f);
	}

	for (auto gm : m_gmList)
	{
		gm->Update();
	}
}

ue::CVector3 Player::GetPos()
{
	return m_chara.GetPos();
}
