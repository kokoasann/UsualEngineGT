#include "stdafx.h"
#include "Player.h"
#include "Cannon/Cannon.h"

Player::Player()
{
	m_pad = &ue::GamePad(0);

	m_anim[PA_idol].Load(L"Assets/model/Player/player_idol.tka");
	m_anim[PA_idol].SetLoopFlag(true);
	m_anim[PA_walk].Load(L"Assets/model/Player/player_walk.tka");
	m_anim[PA_walk].SetLoopFlag(true);
	m_anim[PA_walkFast].Load(L"Assets/model/Player/player_walkFast.tka");
	m_anim[PA_walkFast].SetLoopFlag(true);
	m_anim[PA_dush].Load(L"Assets/model/Player/player_dush.tka");
	m_anim[PA_dush].SetLoopFlag(true);

	ue::SkinModelRender* model = ue::NewGO<ue::SkinModelRender>(0);
	model->Init(L"Assets/model/Player.cmo",m_anim,PA_num,ue::enFbxUpAxisY);
	model->SetPos({ 0,100,0 });
	model->SetSca(ue::CVector3::One()*20.f);
	model->SetIsShadowCaster(true);
	model->SetIsShadowReciever(true);
	m_camera.Init(this);
	m_chara.Init(model, 20, 40, {0,-2.f,0});

	m_motion.Init(this, &m_chara, m_camera.GetCamera(), m_anim,m_pad);

	model->GetAnimation().Play(0);

	m_gmList[0] = &m_camera;
	m_gmList[1] = &m_chara;
	m_gmList[2] = &m_motion;

	m_cannonMesh.Init(L"Assets/model/CannonDummy.cmo", ue::enFbxUpAxisZ);
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
	if (ue::GamePad(0).IsTrigger(ue::enButtonX))
	{
		auto can = ue::NewGO<Cannon>(0);
		
		can->Init(m_cannonMesh, m_chara.GetPos()+ (m_chara.GetDir() * 80.f), m_chara.GetRot());
		m_cannons.push_back(can);
	}
	for (auto gm : m_gmList)
	{
		gm->Update();
	}
}