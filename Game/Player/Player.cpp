#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	m_anim[0].Load(L"Assets/model/Player/player_idol.tka");
	m_anim[0].SetLoopFlag(true);
	m_anim[1].Load(L"Assets/model/Player/player_walk.tka");
	m_anim[1].SetLoopFlag(true);
	m_anim[2].Load(L"Assets/model/Player/player_dush.tka");
	m_anim[2].SetLoopFlag(true);
	ue::SkinModelRender* model = ue::NewGO<ue::SkinModelRender>(0);
	model->Init(L"Assets/model/Player.cmo",m_anim,3,ue::enFbxUpAxisY);
	model->SetPos({ 0,50,0 });
	model->SetSca(ue::CVector3::One()*20.f);
	m_camera.Init(this);
	m_chara.Init(model, 20, 40, {0,0.f,0});
	//m_chara.FindBone(L"Bone_L.003", Character::BK_FootL, true, 3, 10.f);
	//m_chara.FindBone(L"Bone_R.003", Character::BK_FootR, true, 3, 10.f);
	//m_chara.FindBone(L"Bone.003_L.003", Character::BK_HandL, true, 3, 1);
	//m_chara.FindBone(L"Bone.003_R.003", Character::BK_HandR, true, 3, 1);
	//m_chara.FindBone(L"Bone_L.005", Character::BK_None, true, 2, 5);
	//m_chara.FindBone(L"Bone_R.005", Character::BK_None, true, 2, 5);

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
	if ((abs(x) + abs(y)) / 2.f > 0.01f)
	{
		if (!m_isWalk)
		{
			auto cam = m_camera.GetCamera();
			auto f = cam->GetForward() * y * 100;
			auto r = cam->GetRight() * x * 100;
			auto move = f + r;
			m_chara.SetMove(move);

			m_chara.PlayAnim(1, 1, Character::AM_Move);
			m_isWalk = true;
		}
	}
	else if (m_isWalk)
	{
		m_chara.PlayAnim(0, 1, Character::AM_None);
		m_isWalk = false;
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
