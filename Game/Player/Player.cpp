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
	model->SetSca({ 10,10,10 });
	m_camera.Init(this);
	m_chara.Init(model, 10, 20, {0,-1.5f,0});
	m_chara.FindBone(L"Bone_L.004", Character::BK_FootL, true, 3, 3.f);
	m_chara.FindBone(L"Bone_R.004", Character::BK_FootR, true, 3, 3.f);
	//m_chara.FindBone(L"Bone.003_L.003", Character::BK_HandL, true, 3, 1);
	//m_chara.FindBone(L"Bone.003_R.003", Character::BK_HandR, true, 3, 1);
	//m_chara.FindBone(L"Bone_L.005", Character::BK_None, true, 2, 1);
	//m_chara.FindBone(L"Bone_R.005", Character::BK_None, true, 2, 1);
	model->GetAnimation().Play(1);
	m_gmList[0] = &m_camera;
	m_gmList[1] = &m_chara;
	
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

	for (auto gm : m_gmList)
	{
		gm->Update(); 
	}
}

ue::CVector3 Player::GetPos()
{
	return m_chara.GetPos();
}
