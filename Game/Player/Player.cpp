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
	model->SetPos({ 0,50,0 });
	model->SetSca(ue::CVector3::One()*20.f);
	model->SetIsShadowCaster(true);
	model->SetIsShadowReciever(true);
	m_camera.Init(this);
	m_chara.Init(model, 20, 40, {0,-2.f,0});
	m_chara.FindBone(L"Bone_L.003", Character::BK_FootL, true, 3, 13.f);
	m_chara.FindBone(L"Bone_R.003", Character::BK_FootR, true, 3, 13.f);
	//m_chara.FindBone(L"Bone.003_L.003", Character::BK_HandL, true, 3, 1);
	//m_chara.FindBone(L"Bone.003_R.003", Character::BK_HandR, true, 3, 1);
	//m_chara.FindBone(L"Bone_L.005", Character::BK_None, true, 2, 4);
	//m_chara.FindBone(L"Bone_R.005", Character::BK_None, true, 2, 4);

	m_chara.SetMoveFunc([&](auto & move) {return; });
	m_chara.SetRotateFunc([&](auto & rote) {return; });

	model->GetAnimation().Play(1);
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
			m_chara.SetAllIKRub(0.5f);
			
		}
		
		auto cam = m_camera.GetCamera();
		auto f = cam->GetForward() * y * 500;
		auto r = cam->GetRight() * x * 500;
		auto move = f + r;
		m_chara.SetMove(move);
		if (move.Length() > 0.1f)
		{
			move.Normalize();
			float f = move.Dot(m_dir);
			float rad = acos(f);
			ue::CQuaternion add;
			add.SetRotation(ue::CVector3::AxisY(),rad);
			add.Multiply(m_chara.GetRot());
			m_chara.SetRotate(add);
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
