#include "stdafx.h"
#include "CharacterMoveMotion.h"
#include "Character.h"

// 30fps MAXやBlenderでアニメーションを出力するときに1フレーム当たり1/30を使っているので、それに合わせるため。
#define FRAME_RATE 0.03333333333f
// 25% 歩くときや走るときのモーションの右足を出し始めて右足が地に付くまでのちょうど真ん中の位置。
#define ANM_25PER 0.25f
// 75% 歩くときや走るときのモーションの左足を出し始めて左足が地に付くまでのちょうど真ん中の位置。
#define ANM_75PER 0.75f

void CharacterMoveMotion::Release()
{
	m_chara = nullptr;
	m_anim = nullptr;
	m_actionMode = AM_None;
	m_doPlayAnim = 0;

	m_time = 0.f;
	m_isWalk = false;

	m_playingAnim = 0;
	m_oldPlayAnim = 0;
	m_idolNum = 0;
	m_animLugBase = 0.6f;
	
	m_animLug_2idle = 0.0f;
	
	m_moved = ue::CVector3::AxisZ();
	
	m_speed = 0.f;
	m_oldSpeedBuff = 0.0f;
	m_oldSpeed = 0.0f;
	
	m_changeWalk = CW_slow2fast;

	m_footL = nullptr;
	m_footR = nullptr;
	m_isJustFoot = false;
}

void CharacterMoveMotion::OnDestroy()
{
	Release();
}

void CharacterMoveMotion::InitBone(ue::Bone* footL, ue::Bone* footR, bool isIKModeFoot)
{
	m_footL = footL;
	m_footR = footR;
	ue::Animation& anim = m_chara->GetAnimation();
	if (isIKModeFoot)
	{
		anim.SetIKMode(ue::IK::enMode_Foot, footL);
		anim.SetIKMode(ue::IK::enMode_Foot, footR);
	}
	else
	{
		anim.SetIKMode(ue::IK::enMode_Normal, footL);
		anim.SetIKMode(ue::IK::enMode_Normal, footR);
	}
	m_footLIK = m_chara->GetFootRIK();
}

void CharacterMoveMotion::InitStartUpFoot(float upMax,float upspeed)
{
	m_isStartUpFoot = true;
	m_startUpMax = upMax;
	m_startUpSpeed = upspeed;
}

void CharacterMoveMotion::Update()
{
	float delTime = ue::gameTime()->GetDeltaTime();
	if (m_speed > 0.f)
	{
		m_chara->SetIKSpeed(1.0f);
		Move(delTime, m_doPlayAnim, m_speed, m_actionMode);
	}
	else if(m_time > 0.f && !m_isJustFoot)
	{
		Walk2Idle(delTime);
	}
	else
	{
		if (m_isJustFoot)
		{
			m_chara->Start_JustFoot();
			m_isJustFoot = false;
		}
		//JustFoot(delTime);
	}
}

void CharacterMoveMotion::Move(float delTime, PlayAnim pa, float movespeed, ActionMode am)
{
	if (m_playingAnim != pa)		//アイドルから歩き、走りから早歩き等の移動方法が変化するとここに入る。
	{
		float ntime = 0.f;
		float animLug = m_animLugBase;
		ChangePlayingAnim(pa);
		if (m_playingAnim != m_idolNum)
		{
			//これから再生するアニメーションの再生開始時間を計算。
			const auto& anm = m_chara->GetAnimation();
			float fc = anm.GetFrameNum();			//現在再生中のアニメーションのフレーム数。
			float fn = anm.GetCurrentFrameNo();		//現在再生中のアニメーションのフレーム番号。
			float fnRate = fn / fc;					//現在再生中のフレーム番号がフレーム数に対して何％なのか調べる。
			float nfc = m_anim[pa].GetFrameNum();	//これから再生するアニメーションのフレーム数。
			float nfn = nfc * fnRate;				//これから再生するアニメーションのフレーム番号を調べる。
			ntime = nfn * FRAME_RATE;				//アニメーションのスタート時間。

			//
			auto spar = m_oldSpeed / movespeed;
			m_time = m_animLugBase * spar;
			if (m_animLugBase > m_time)	//遅い移動から速い移動。
			{
				animLug = m_animLugBase - m_time;
				m_changeWalk = CW_slow2fast;
			}
			else													//速い移動から遅い移動。
			{
				animLug = m_time - m_animLugBase;
				m_changeWalk = CW_fast2slow;
			}
		}
		m_chara->PlayAnim(pa, animLug, ntime, am);
		m_isWalk = true;
		m_isUpfoot = true;
	}
	if (m_isUpfoot)
	{
		const auto& anm = m_chara->GetAnimation();
		float fc = anm.GetFrameNum();			//現在再生中のアニメーションのフレーム数。
		float fn = anm.GetCurrentFrameNo();		//現在再生中のアニメーションのフレーム番号。
		float fnRate = fn / fc;					//現在再生中のフレーム番号がフレーム数に対して何％なのか調べる。
		if (fnRate <= 0.5)
		{
			m_startUpNow += m_startUpSpeed*delTime;
			if (m_startUpNow > m_startUpMax)
				m_startUpNow = m_startUpMax;
			m_footLIK->SetOffset({ 0.f,m_startUpNow ,0.f});
		}
		else
		{
			m_startUpNow -= m_startUpSpeed*delTime;
			m_footLIK->SetOffset({ 0.f,m_startUpNow ,0.f });
			if (m_startUpNow < 0.f)
			{
				m_startUpNow = 0.f;
				m_isUpfoot = false;
			}
		}
	}

	//スピードの計算。
	float speed = 0.0f;
	if (m_changeWalk == CW_slow2fast && m_time < m_animLugBase)//遅い移動から速い移動になった時のスピードの加速
	{
		speed = movespeed * (m_time / m_animLugBase);
		m_time += delTime;
	}
	else if (m_changeWalk == CW_fast2slow && m_time >= m_animLugBase)//速い移動から遅い移動になった時のスピードの減速
	{
		speed = movespeed * (m_time / m_animLugBase);
		m_time -= delTime;
	}
	else
	{
		speed = movespeed;
	}
	m_oldSpeedBuff = speed;

	if (AM_Move == am)
	{
		return;
	}

	//実際に移動。

	auto move = m_chara->GetDir();
	move.y = 0;
	move.Normalize();
	m_chara->SetIKOffset(move * speed * delTime);
	m_chara->SetMove(move * speed);
	m_moved = move * speed;
}

void CharacterMoveMotion::Walk2Idle(float delTime)
{
	if (m_isWalk)
	{
		const auto& anm = m_chara->GetAnimation();
		float fc = anm.GetFrameNum();			//現在再生中のアニメーションのフレーム数。
		float fn = anm.GetCurrentFrameNo();		//現在再生中のアニメーションのフレーム番号。
		float fnRate = fn / fc;					//現在再生中のフレーム番号がフレーム数に対して何％なのか調べる。
		if (fnRate <= ANM_25PER)
		{
			float nfn = fc * ANM_75PER;
			float len = nfn - fn;
			m_animLug_2idle = len * FRAME_RATE;
		}
		else if (fnRate <= ANM_75PER)
		{
			float len = fc - fn;
			m_animLug_2idle = len * FRAME_RATE;
			float nfn = fc * ANM_25PER * FRAME_RATE;
			m_animLug_2idle += nfn;
		}
		else
		{
			float len = fc - fn;
			m_animLug_2idle = len * FRAME_RATE;
			float nfn = fc * ANM_75PER * FRAME_RATE;
			m_animLug_2idle += nfn;
		}
		if (m_animLug_2idle == 0.f)
			return;
		ChangePlayingAnim(m_idolNum);
		m_moved = m_chara->GetDir() * m_oldSpeed;

		//m_chara->PlayAnim(m_idolNum, m_animLug_2idle);
		
		m_isWalk = false;
		//m_chara->SetAllIKRub(0.0f);
		//m_chara->SetIKOffset(ue::CVector3::Zero());
		m_time = m_animLug_2idle;

		
	//m_chara->SetMoveFunc(m_noneMF);
		//m_isJustedEnd = false;
		//m_isJustedStart = false;
	//m_justTime = 0.f;
	//m_isStartJustFoot = false;
		m_chara->PlayAnim(m_idolNum, 1.f,0.f, AM_Move);
		m_isJustFoot = true;
	}
	auto move = m_moved * ((m_time) / m_animLug_2idle);
	//m_chara->SetIKOffset(move * delTime);
	m_chara->SetMove(move);
	m_time -= delTime;
	if (m_time <= 0.f)
	{
		
	}
}
void CharacterMoveMotion::ChangePlayingAnim(PlayAnim pa)
{
	m_oldSpeed = m_oldSpeedBuff;
	if (m_playingAnim == m_idolNum)
		m_oldSpeed = 0.0f;
	m_oldPlayAnim = m_playingAnim;
	m_playingAnim = pa;
}

void CharacterMoveMotion::BreakAnim()
{
	m_speed = 0.0f;
	m_time = 0.0f;

	m_doPlayAnim = m_idolNum;
	ChangePlayingAnim(m_idolNum);
}
