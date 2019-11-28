#include "stdafx.h"
#include "PlayerMotion.h"
#include "Player.h"
#include "Character/Character.h"

void PlayerMotion::Init(Player* player, Character* chara, ue::Camera* cam, ue::AnimationClip* anim, ue::Pad* pad)
{
	m_player = player;
	m_chara = chara;
	m_camera = cam;
	m_anim = anim;
	m_pad = pad;

	m_footL = m_chara->FindBone(L"Bone_L.003", Character::BK_FootL, true, 3, 13.f);
	m_footR = m_chara->FindBone(L"Bone_R.003", Character::BK_FootR, true, 3, 13.f);
	m_chara->SetMoveFunc([&](auto & move) {return;	});
	m_chara->SetRotateFunc([&](auto & rote) {return; });
}

void PlayerMotion::InitBone(ue::Bone* footL, ue::Bone* footR)
{
	m_footL = footL;
	m_footR = footR;
}

void PlayerMotion::Update()
{
	ue::CVector2 stick;
	stick.x = m_pad->GetLStickXF();
	stick.y = m_pad->GetLStickYF();
	float deltime = ue::gameTime()->GetDeltaTime();

	if (fabsf(stick.x) > 0.01f || fabsf(stick.y) > 0.01f)	//stick入力がある。つまり移動。
	{
		m_chara->SetIKSpeed(1.0f);
		if (m_pad->IsPress(ue::enButtonB))
		{
			Move(stick, PA_dush, m_dushSpeed, Character::AM_Move);
		}
		else if (stick.Length() >= 0.5f)
		{
			Move(stick, PA_walkFast, m_walkFastSpeed, Character::AM_Move);
		}
		else
		{
			Move(stick, PA_walk, 0.f, Character::AM_Move);
			m_chara->SetDefaultMoveFunc();
		}
	}
	else if (m_lugTime > 0.f)	//移動からアイドルにかけての処理。
	{
		Walk2Idle(deltime);
	}
	else	//止まり切った後に足の位置を調整する。
	{
		JustFoot(deltime);
	}
}

void PlayerMotion::Move(const ue::CVector2& padStick, PlayerAnim pa, float movespeed,int am)
{
	float deltime = ue::gameTime()->GetDeltaTime();
	//x = m_pad->IsPress(ue::enButtonA);
	if (!m_isWalk || m_PlayingAnim != pa)
	{
		float ntime = 0.f;
		if (!m_isWalk)
		{
			auto& anm = m_chara->GetAnimation();
			float fc = anm.GetFrameNum();			//現在再生中のアニメーションのフレーム数。
			float fn = anm.GetCurrentFrameNo();		//現在再生中のアニメーションのフレーム番号。
			float fnRate = fn / fc;					//現在再生中のフレーム番号がフレーム数に対して何％なのか調べる。
			float nfc = m_anim[pa].GetFrameNum();	//これから再生するアニメーションのフレーム数。
			float nfn = nfc * fnRate;				//これから再生するアニメーションのフレーム番号を調べる。
			ntime = nfn * 1.f / 30.f;				//アニメーションのスタート時間。
		}
		m_chara->PlayAnim(pa, m_animLug, ntime, (Character::ActionMode)am);
		m_isWalk = true;
		
		ChangePlayingAnim(pa);

		m_chara->SetAllIKRub(1.0f);

		m_chara->SetMoveFunc([&](auto & move) {return;	});
	}

	float speed = 0.0f;
	if (m_lugTime < m_animLug)
	{
		if(m_lugTime != 0.f)
			speed = movespeed * (m_lugTime / m_animLug) + max(m_oldSpeed - movespeed,0.f);
		else
			speed = max(m_oldSpeed - movespeed, 0.f);
		
		m_lugTime += deltime;
	}
	else
	{
		speed = movespeed;
	}
	m_oldSpeedBuff = speed;

	auto f = m_camera->GetForward() * padStick.y;
	auto r = m_camera->GetRight() * padStick.x;
	auto move = f + r;
	move.y = 0;
	move.Normalize();

	m_chara->SetIKOffset(move * speed * deltime);
	m_chara->SetMove(move * speed);
	m_moved = move * speed;
	if (move.Length() > 0.1f)	//こっから回転。
	{
		move.Normalize();
		float rad = atan2(move.x, move.z);
		ue::CQuaternion add;
		add.SetRotation(ue::CVector3::AxisY(), rad);
		m_dir = move;
		m_chara->SetRotation(add);
	}
}

void PlayerMotion::Walk2Idle(float delTime)
{
	if (m_isWalk)
	{
		auto& anm = m_chara->GetAnimation();
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
			float nfn = fc * 0.25f * (1.f / 30.f);
			m_animLug_2idle += nfn;
		}

		m_chara->PlayAnim(PA_idol, m_animLug_2idle, Character::AM_None);
		m_isWalk = false;
		m_chara->SetAllIKRub(0.0f);
		m_chara->SetIKOffset(ue::CVector3::Zero());
		m_lugTime = m_animLug_2idle;

		m_chara->SetMoveFunc([&](auto & move) {return;	});
		m_isJustedL = false;
		m_isJustedR = false;
		m_justTimeL = 0.f;
		m_justTimeR = 0.f;

		m_chara->SetIKOffset(ue::CVector3::Zero());
	}
	auto move = m_moved * ((m_lugTime) / m_animLug_2idle);
	m_chara->SetIKOffset(move * delTime);
	m_chara->SetMove(move);
	m_lugTime -= delTime;
}

void PlayerMotion::JustFoot(float delTime)
{
	
	
	if (!m_isJustedL)
	{
		auto moveL = m_footL->GetMove().Length();
		if (m_justTimeL > 0.2f || moveL < 10.f)
		{
			m_isJustedL = true;
			m_chara->SetIKSpeed(0.2f, m_footL);
		}
		else if (m_justTimeL < 0.1f)
		{
			auto up = m_dir * -1.f;

			up.y += 2.f;
			up.Normalize();
			m_chara->SetIKOffset(up * 40.f, m_footL);
			m_chara->SetIKSpeed(0.2f, m_footL);
		}
		m_justTimeL += delTime;
	}
	else if (!m_isJustedR)
	{
		auto moveR = m_footR->GetMove().Length();
		if (m_justTimeR > 0.1f || moveR < 10.f)
		{
			m_isJustedR = true;
			m_chara->SetIKSpeed(0.5f, m_footR);
		}
		auto up = m_dir * -1.f;
		up.y += 2.f;
		up.Normalize();
		m_chara->SetIKOffset(up * 40.f, m_footR);
		m_chara->SetIKSpeed(0.2f, m_footR);
		m_justTimeR += delTime;
	}
}

void PlayerMotion::ChangePlayingAnim(PlayerAnim pa)
{
	m_PlayingAnim = pa;
	if (pa != PA_idol)
		m_oldSpeed = m_oldSpeedBuff;
	else
		m_oldSpeed = 0.f;
}
