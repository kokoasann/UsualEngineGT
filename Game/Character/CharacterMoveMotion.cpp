#include "stdafx.h"
#include "CharacterMoveMotion.h"
#include "Character.h"

// 30fps MAX��Blender�ŃA�j���[�V�������o�͂���Ƃ���1�t���[��������1/30���g���Ă���̂ŁA����ɍ��킹�邽�߁B
#define FRAME_RATE 0.03333333333f
// 25% �����Ƃ��⑖��Ƃ��̃��[�V�����̉E�����o���n�߂ĉE�����n�ɕt���܂ł̂��傤�ǐ^�񒆂̈ʒu�B
#define ANM_25PER 0.25f
// 75% �����Ƃ��⑖��Ƃ��̃��[�V�����̍������o���n�߂č������n�ɕt���܂ł̂��傤�ǐ^�񒆂̈ʒu�B
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
	if (m_playingAnim != pa)		//�A�C�h����������A���肩�瑁�������̈ړ����@���ω�����Ƃ����ɓ���B
	{
		float ntime = 0.f;
		float animLug = m_animLugBase;
		ChangePlayingAnim(pa);
		if (m_playingAnim != m_idolNum)
		{
			//���ꂩ��Đ�����A�j���[�V�����̍Đ��J�n���Ԃ��v�Z�B
			const auto& anm = m_chara->GetAnimation();
			float fc = anm.GetFrameNum();			//���ݍĐ����̃A�j���[�V�����̃t���[�����B
			float fn = anm.GetCurrentFrameNo();		//���ݍĐ����̃A�j���[�V�����̃t���[���ԍ��B
			float fnRate = fn / fc;					//���ݍĐ����̃t���[���ԍ����t���[�����ɑ΂��ĉ����Ȃ̂����ׂ�B
			float nfc = m_anim[pa].GetFrameNum();	//���ꂩ��Đ�����A�j���[�V�����̃t���[�����B
			float nfn = nfc * fnRate;				//���ꂩ��Đ�����A�j���[�V�����̃t���[���ԍ��𒲂ׂ�B
			ntime = nfn * FRAME_RATE;				//�A�j���[�V�����̃X�^�[�g���ԁB

			//
			auto spar = m_oldSpeed / movespeed;
			m_time = m_animLugBase * spar;
			if (m_animLugBase > m_time)	//�x���ړ����瑬���ړ��B
			{
				animLug = m_animLugBase - m_time;
				m_changeWalk = CW_slow2fast;
			}
			else													//�����ړ�����x���ړ��B
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
		float fc = anm.GetFrameNum();			//���ݍĐ����̃A�j���[�V�����̃t���[�����B
		float fn = anm.GetCurrentFrameNo();		//���ݍĐ����̃A�j���[�V�����̃t���[���ԍ��B
		float fnRate = fn / fc;					//���ݍĐ����̃t���[���ԍ����t���[�����ɑ΂��ĉ����Ȃ̂����ׂ�B
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

	//�X�s�[�h�̌v�Z�B
	float speed = 0.0f;
	if (m_changeWalk == CW_slow2fast && m_time < m_animLugBase)//�x���ړ����瑬���ړ��ɂȂ������̃X�s�[�h�̉���
	{
		speed = movespeed * (m_time / m_animLugBase);
		m_time += delTime;
	}
	else if (m_changeWalk == CW_fast2slow && m_time >= m_animLugBase)//�����ړ�����x���ړ��ɂȂ������̃X�s�[�h�̌���
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

	//���ۂɈړ��B

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
		float fc = anm.GetFrameNum();			//���ݍĐ����̃A�j���[�V�����̃t���[�����B
		float fn = anm.GetCurrentFrameNo();		//���ݍĐ����̃A�j���[�V�����̃t���[���ԍ��B
		float fnRate = fn / fc;					//���ݍĐ����̃t���[���ԍ����t���[�����ɑ΂��ĉ����Ȃ̂����ׂ�B
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
