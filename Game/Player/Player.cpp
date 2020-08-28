#include "stdafx.h"
#include "Player.h"
#include "Cannon/Cannon.h"

struct SweepResultCannon : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	btCollisionObject* me = nullptr;
	Cannon* hitCannon = nullptr;
	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (!(convexResult.m_hitCollisionObject->getUserIndex() & CUI_Cannon))
		{
			return 0.0f;
		}
		if(hitCannon == nullptr)
			hitCannon = (Cannon*)convexResult.m_hitCollisionObject->getUserPointer();
		isHit = true;
		return 1.0f;
	}
};

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
	model->SetPos({ -30,0.1f,0 });
	model->SetSca(ue::CVector3::One()*2.f);
	model->SetIsShadowCaster(true);
	model->SetIsShadowReciever(true);
	m_camera.Init(this);
	m_chara.Init(model, 2, 4, {0,-0.2f,0},false);

	m_motion.Init(this, &m_chara, m_camera.GetCamera(), m_anim,m_pad);

	model->GetAnimation().Play(0);

	m_gmList[0] = &m_camera;
	m_gmList[1] = &m_chara;
	m_gmList[2] = &m_motion;

	m_cannonMesh.Init(L"Assets/model/CannonDummy.cmo", ue::enFbxUpAxisZ);

	m_collider.Create(10);
}

Player::~Player()
{
}

void Player::Release()
{
}

void Player::OnDestroy()
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
		bool isSetting = true;
		auto newc = m_chara.GetPos() + (m_chara.GetDir() * 8.f);
		for (auto c:m_cannons)
		{
			auto cp = c->GetPos();
			if ((newc - cp).Length() < 25.f)
			{
				isSetting = false;
				break;
			}
		}
		if (isSetting)
		{
			auto can = ue::NewGO<Cannon>(0);

			can->Init(m_cannonMesh, m_chara.GetPos() + (m_chara.GetDir() * 8.f), m_chara.GetRot());
			can->SetDir(m_chara.GetDir());
			m_cannons.push_back(can);
		}
		else
		{
			auto pos = m_chara.GetPos();
			auto dir = m_chara.GetDir();
			btTransform bstart, bend;
			bstart.setIdentity();
			bend.setIdentity();
			auto bpos = btVector3(pos.x, pos.y, pos.z);
			bstart.setOrigin(bpos);
			bend.setOrigin(bpos + (btVector3(dir.x, dir.y, dir.z) * 240));
			SweepResultCannon sr;
			ue::Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
			if (sr.isHit)
			{
				m_camera.CannonMode(sr.hitCannon);
				m_isSettingCannon = true;
				
				m_motion.ToIdole();
			}
		}
	}
	if (m_isSettingCannon)
	{
		if (m_pad->IsTrigger(ue::enButtonB))
		{
			m_camera.NormalMode();
			m_isSettingCannon = false;
		}
		m_motion.UpdateMotion();
		m_camera.Update();
	}
	else
	{
		if (m_pad->IsTrigger(ue::enButtonRB1))
		{
			for (auto& can : m_cannons)
			{
				can->Fire();
			}
		}
		for (auto gm : m_gmList)
		{
			gm->Update();
		}
	}
}