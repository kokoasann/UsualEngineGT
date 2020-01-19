#include "stdafx.h"
#include "PlayerMotion.h"
#include "Player.h"
#include "Character/Character.h"
#include "Character/CharacterConst.h"

void PlayerMotion::Init(Player* player, Character* chara, ue::Camera* cam, ue::AnimationClip* anim, ue::Pad* pad)
{
	m_player = player;
	m_chara = chara;
	m_camera = cam;
	m_anim = anim;
	m_pad = pad;

	m_footL = m_chara->FindBone(L"Bone_L.003", BK_FootL, true, 3, 10.f);
	m_footR = m_chara->FindBone(L"Bone_R.003", BK_FootR, true, 3, 10.f);
	m_noneMF = [&](auto& pos) {return;	};
	m_noneRF = [&](auto& rote) {return; };

	//m_chara->SetMoveFunc(m_noneMF);
	//m_chara->SetRotateFunc(m_noneRF);
	auto& anime = m_chara->GetAnimation();
	float v = 50.f;
	anime.SetIKGravity(9.8f*v, m_footL);
	anime.SetIKGravity(9.8f*v, m_footR);

	m_charaMove.Init(m_chara, m_anim);
	m_charaMove.InitBone(m_footL, m_footR);

	m_charaRotate.Init(m_chara,CharacterRotateMotion::RM_Lerp,15.f);
	m_chara->Init_JustFoot(2.f, 40.f, 0.4f, 0.5f);
}


void PlayerMotion::Update()
{
	ue::CVector2 stick;
	stick.x = m_pad->GetLStickXF();
	stick.y = m_pad->GetLStickYF();
	float deltime = ue::gameTime()->GetDeltaTime();

	if (stick.Length() > 0.0001f)
	{
		auto f = m_camera->GetForward() * stick.y;
		auto r = m_camera->GetRight() * stick.x;
		auto move = f + r;
		m_charaRotate.NextRotate(move, 0);
	}

	if (fabsf(stick.x) > 0.01f || fabsf(stick.y) > 0.01f)	//stick入力がある。つまり移動。
	{
		if (m_pad->IsPress(ue::enButtonB))		//ダッシュ
		{
			m_charaMove.NextPlayAnim(PA_dush, m_dushSpeed, AM_None);
		}
		else if (stick.Length() >= 0.5f)				//早歩き
		{
			m_charaMove.NextPlayAnim(PA_walkFast, m_walkFastSpeed, AM_None);
		}
		else														//歩き
		{
			m_charaMove.NextPlayAnim(PA_walk, m_walkSpeed, AM_Move);
		}
		
	}
	else
	{
		m_charaMove.NextPlayAnim(PA_idol, 0, AM_None);
	}
	m_charaRotate.Update();
	m_charaMove.Update();
	return;

	if (fabsf(stick.x) > 0.01f || fabsf(stick.y) > 0.01f)	//stick入力がある。つまり移動。
	{
		m_chara->SetIKSpeed(1.0f);
		if (m_pad->IsPress(ue::enButtonB))		//ダッシュ
		{
			Move(stick, PA_dush, m_dushSpeed, AM_Move);
		}
		else if (stick.Length() >= 0.5f)				//早歩き
		{
			Move(stick, PA_walkFast, m_walkFastSpeed, AM_Move);
		}
		else														//歩き
		{
			Move(stick, PA_walk, 0.f, AM_Move);
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

	if (!m_isWalk || m_PlayingAnim != pa)		//アイドルから歩き、走りから早歩き等の移動方法が変化するとここに入る。
	{
		ChangePlayingAnim(pa);
		float ntime = 0.f;
		if (m_PlayingAnim != pa)
		{
			//これから再生するアニメーションの再生開始時間を計算。
			auto& anm = m_chara->GetAnimation();
			float fc = anm.GetFrameNum();			//現在再生中のアニメーションのフレーム数。
			float fn = anm.GetCurrentFrameNo();		//現在再生中のアニメーションのフレーム番号。
			float fnRate = fn / fc;					//現在再生中のフレーム番号がフレーム数に対して何％なのか調べる。
			float nfc = m_anim[pa].GetFrameNum();	//これから再生するアニメーションのフレーム数。
			float nfn = nfc * fnRate;				//これから再生するアニメーションのフレーム番号を調べる。
			ntime = nfn * 1.f / 30.f;				//アニメーションのスタート時間。

			//
			auto spar = m_oldSpeed / movespeed;
			m_lugTime = m_animLugBase * spar;
			if (m_animLugBase > m_lugTime)	//遅い移動から速い移動。
			{
				m_animLug = m_animLugBase - m_lugTime;
				m_changeWalk = CW_slow2fast;
			}
			else													//速い移動から遅い移動。
			{
				m_animLug = m_lugTime - m_animLugBase;
				m_changeWalk = CW_fast2slow;
			}
		}
		m_chara->PlayAnim(pa, m_animLug, ntime, (ActionMode)am);
		m_isWalk = true;
		//m_chara->SetAllIKRub(1.0f);
		//m_chara->SetMoveFunc(m_noneMF);
	}

	//スピードの計算。
	float speed = 0.0f;
	if (m_changeWalk == CW_slow2fast && m_lugTime < m_animLugBase)//遅い移動から速い移動になった時のスピードの加速
	{
		speed = movespeed * (m_lugTime / m_animLugBase);
		m_lugTime += deltime;
	}
	else if (m_changeWalk == CW_fast2slow && m_lugTime >= m_animLugBase)//速い移動から遅い移動になった時のスピードの減速
	{
		speed = movespeed * (m_lugTime / m_animLugBase);
		m_lugTime -= deltime;
	}
	else
	{
		speed = movespeed;
	}
	m_oldSpeedBuff = speed;

	//実際に移動。
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
			float nfn = fc * 0.75f;
			float len = nfn - fn;
			m_animLug_2idle = len * (1.f / 30.f);
		}
		else if (fnRate <= 0.75f)
		{
			float len = fc - fn;
			m_animLug_2idle = len * (1.f / 30.f);
			float nfn = fc * 0.25f * (1.f / 30.f);
			m_animLug_2idle += nfn;
		}
		else
		{
			float len = fc - fn;
			m_animLug_2idle = len * (1.f / 30.f);
			float nfn = fc * 0.75f * (1.f / 30.f);
			m_animLug_2idle += nfn;
		}
		if (m_animLug_2idle == 0.f)
			return;

		if (m_PlayingAnim == PA_walk)
			m_moved = m_dir * m_walkSpeed;

		m_chara->PlayAnim(PA_idol, m_animLug_2idle, AM_None);
		m_isWalk = false;
		//m_chara->SetAllIKRub(0.0f);
		m_chara->SetIKOffset(ue::CVector3::Zero());
		m_lugTime = m_animLug_2idle;

		ChangePlayingAnim(PA_idol);
		m_chara->SetMoveFunc(m_noneMF);
		m_isJustedEnd = false;
		m_isJustedStart = false;
		m_justTimeEnd = 0.f;
		m_justTimeStart = 0.f;
		m_isStartJustFoot = false;
	}
	auto move = m_moved * ((m_lugTime) / m_animLug_2idle);
	//m_chara->SetIKOffset(move * delTime);
	m_chara->SetMove(move);
	m_lugTime -= delTime;
}

void PlayerMotion::JustFoot(float delTime)
{
	if (!m_isStartJustFoot)//遠い足の方から先に整える
	{
		auto moveL = m_footL->GetMove().Length();
		auto moveR = m_footR->GetMove().Length();

		if (moveL > moveR)
			m_startJustFoot = JF_footL;
		else
			m_startJustFoot = JF_footR;
		m_isStartJustFoot = true;
	}
	ue::Bone* startBone = nullptr;
	ue::Bone* endBone = nullptr;

	switch (m_startJustFoot)
	{
	case JF_footL://左足からスタート
		startBone = m_footL;
		endBone = m_footR;
		break;
	case JF_footR://右足からスタート
		startBone = m_footR;
		endBone = m_footL;
		break;
	}

	if (!m_isJustedStart)//最初に出す足の処理。
	{
		auto move = startBone->GetMove().Length();
		if (m_justTimeStart > 0.2f || move < 10.f)
		{
			m_isJustedStart = true;
			m_chara->SetIKSpeed(0.5f, startBone);
		}
		else if (m_justTimeStart < 0.1f)
		{
			auto up = m_dir * -1.f;

			up.y += 2.f;
			up.Normalize();
			m_chara->SetIKOffset(up * 40.f, startBone);
			m_chara->SetIKSpeed(0.4f, startBone);
		}
		m_justTimeStart += delTime;
	}
	else if (!m_isJustedEnd)//最後に出す足の処理。
	{
		auto move = endBone->GetMove().Length();
		if (m_justTimeEnd > 0.1f || move < 10.f)
		{
			m_isJustedEnd = true;
			m_chara->SetIKSpeed(0.5f, endBone);
		}
		auto up = m_dir * -1.f;
		up.y += 2.f;
		up.Normalize();
		m_chara->SetIKOffset(up * 40.f, endBone);
		m_chara->SetIKSpeed(0.2f, endBone);
		m_justTimeEnd += delTime;
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
