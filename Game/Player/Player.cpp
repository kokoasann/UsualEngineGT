#include "stdafx.h"
#include "Player.h"

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

	/*m_footL = m_chara.FindBone(L"Bone_L.003", Character::BK_FootL, true, 3, 13.f);
	m_footR = m_chara.FindBone(L"Bone_R.003", Character::BK_FootR, true, 3, 13.f);*/
	//m_chara.FindBone(L"Bone.003_L.003", Character::BK_HandL, true, 3, 1);
	//m_chara.FindBone(L"Bone.003_R.003", Character::BK_HandR, true, 3, 1);
	//m_chara.FindBone(L"Bone_L.005", Character::BK_None, true, 2, 4);
	//m_chara.FindBone(L"Bone_R.005", Character::BK_None, true, 2, 4);

	/*m_chara.SetMoveFunc([&](auto & move) {return;	});
	m_chara.SetRotateFunc([&](auto & rote) {return; });*/


	model->GetAnimation().Play(0);
	m_gmList[0] = &m_camera;
	m_gmList[1] = &m_chara;
	m_gmList[2] = &m_motion;
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
#if 0
	ue::CVector2 stick;
	stick.x = m_pad->GetLStickXF();
	stick.y = m_pad->GetLStickYF();
	float deltime = ue::gameTime()->GetDeltaTime();
	if (fabsf(stick.x) > 0.01f || fabsf(stick.y) > 0.01f)
	{
		m_chara.SetIKSpeed(1.0f);
		if (m_pad->IsPress(ue::enButtonB))
		{
			Move(stick, PA_dush, m_dushSpeed, Character::AM_Move);
		}
		else if(stick.Length() >= 0.5f)
		{
			Move(stick, PA_walkFast, m_walkFastSpeed, Character::AM_Move);
		}
		else
		{
			Move(stick, PA_walk, 0.f, Character::AM_Move);
			m_chara.SetDefaultMoveFunc();
		}
	}
	else if(m_lugTime > 0.f)
	{
		if (m_isWalk)
		{
			auto& anm = m_chara.GetAnimation();
			float fc = anm.GetFrameNum();			//現在再生中のアニメーションのフレーム数。
			float fn = anm.GetCurrentFrameNo();		//現在再生中のアニメーションのフレーム番号。
			float fnRate = fn / fc;					//現在再生中のフレーム番号がフレーム数に対して何％なのか調べる。
			if (fnRate <= 0.25f)
			{
				float nfn = fc * 0.25f;
				float len = nfn - fn;
				m_animLug_2idle = len * (1.f / 30.f);
			}
			else if (fnRate <= 0.75f)
			{
				float nfn = fc * 0.75f;
				float len = nfn - fn;
				m_animLug_2idle = len * (1.f / 30.f);
			}
			else
			{
				float len = fc - fn;
				m_animLug_2idle = len * (1.f / 30.f);
				float nfn = fc * 0.25f *(1.f / 30.f);
				m_animLug_2idle += nfn;
			}

			m_chara.PlayAnim(PA_idol, m_animLug_2idle, Character::AM_None);
			m_isWalk = false;
			m_chara.SetAllIKRub(0.0f);
			m_chara.SetIKOffset(ue::CVector3::Zero());
			m_lugTime = m_animLug_2idle;

			m_chara.SetMoveFunc([&](auto & move) {return;	});
			m_isJustedL = false;
			m_isJustedR = false;
			m_justTimeL = 0.f;
			m_justTimeR = 0.f;
		}
		auto move = m_moved * ((m_lugTime) / m_animLug_2idle);
		m_chara.SetIKOffset(move * deltime);
		m_chara.SetMove(move);
		m_lugTime -= deltime;
	}
	else
	{
		auto moveL = m_footL->GetMove().Length();
		auto moveR = m_footR->GetMove().Length();
		if (!m_isJustedL)
		{
			if (m_justTimeL > 0.05f)
			{
				m_isJustedL = true;
				m_chara.SetIKSpeed(0.5f, m_footL);
			}
			auto up = m_dir*-1.f;
			
			up.y += 2.f;
			up.Normalize();
			m_chara.SetIKOffset(up *40.f, m_footL);
			m_chara.SetIKSpeed(0.2f, m_footL);
			m_justTimeL += deltime;
		}
		else if (!m_isJustedR)
		{
			if (m_justTimeR > 0.1f)
			{
				m_isJustedR = true;
				m_chara.SetIKSpeed(0.5f, m_footR);
			}
			auto up = m_dir*-1.f;
			up.y += 2.f;
			up.Normalize();
			m_chara.SetIKOffset(up* 40.f, m_footR);
			m_chara.SetIKSpeed(0.2f, m_footR);
			m_justTimeR += deltime;
		}
	}
#endif

	for (auto gm : m_gmList)
	{
		gm->Update();
	}
}

void Player::Move(const ue::CVector2& padStick, PlayerAnim pa, float movespeed, Character::ActionMode am)
{
	float deltime = ue::gameTime()->GetDeltaTime();
	//x = m_pad->IsPress(ue::enButtonA);
	if (!m_isWalk || m_PlayingAnim != pa)
	{
		float ntime = 0.f;
		//m_animLug = 0.1f;
		if (!m_isWalk)
		{
			auto& anm = m_chara.GetAnimation();
			float fc = anm.GetFrameNum();			//現在再生中のアニメーションのフレーム数。
			float fn = anm.GetCurrentFrameNo();		//現在再生中のアニメーションのフレーム番号。
			float fnRate = fn / fc;					//現在再生中のフレーム番号がフレーム数に対して何％なのか調べる。
			float nfc = m_anim[pa].GetFrameNum();	//これから再生するアニメーションのフレーム数。
			float nfn = nfc * fnRate;				//これから再生するアニメーションのフレーム番号を調べる。
			ntime = nfn * 1.f / 30.f;				//アニメーションのスタート時間。
		}
		m_chara.PlayAnim(pa, m_animLug,ntime, am);
		m_isWalk = true;
		m_PlayingAnim = pa;
		m_chara.SetAllIKRub(1.0f);

		m_chara.SetMoveFunc([&](auto & move) {return;	});
	}

	float speed = 0.0f;
	if (m_lugTime < m_animLug)
	{
		speed = movespeed * m_lugTime / m_animLug + m_oldSpeed-movespeed;
		m_lugTime += deltime;
	}
	else
	{
		speed = movespeed;
		m_oldSpeed = speed;
	}
	

	auto cam = m_camera.GetCamera();
	auto f = cam->GetForward() * padStick.y;
	auto r = cam->GetRight() * padStick.x;
	auto move = f + r;
	move.y = 0;
	move.Normalize();

	m_chara.SetIKOffset(move * speed * deltime);
	m_chara.SetMove(move*speed);
	m_moved = move*speed;
	if (move.Length() > 0.1f)	//こっから回転。
	{
		move.Normalize();
		float rad = atan2(move.x, move.z);
		ue::CQuaternion add;
		add.SetRotation(ue::CVector3::AxisY(), rad);
		m_dir = move;
		m_chara.SetRotation(add);
	}
}

ue::CVector3 Player::GetPos()
{
	return m_chara.GetPos();
}
