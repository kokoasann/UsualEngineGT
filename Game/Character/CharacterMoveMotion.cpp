#include "stdafx.h"
#include "CharacterMoveMotion.h"
#include "Character.h"

// 30fps MAX��Blender�ŃA�j���[�V�������o�͂���Ƃ���1�t���[��������1/30���g���Ă���̂ŁA����ɍ��킹�邽�߁B
#define FRAME_RATE 0.03333333333f
// 25% �����Ƃ��⑖��Ƃ��̃��[�V�����̉E�����o���n�߂ĉE�����n�ɕt���܂ł̂��傤�ǐ^�񒆂̈ʒu�B
#define ANM_25PER 0.25f
// 75% �����Ƃ��⑖��Ƃ��̃��[�V�����̍������o���n�߂č������n�ɕt���܂ł̂��傤�ǐ^�񒆂̈ʒu�B
#define ANM_75PER 0.75f

void CharacterMoveMotion::Update()
{
	float delTime = ue::gameTime()->GetDeltaTime();
	if (m_speed > 0.f)
	{
		m_chara->SetIKSpeed(1.0f);
		Move(delTime, m_doPlayAnim, m_speed, m_actionMode);
	}
	else if(m_time > 0.f && !m_isStartJustFoot)
	{
		Walk2Idle(delTime);
	}
	else
	{
		JustFoot(delTime);
	}
}

void CharacterMoveMotion::Move(float delTime, PlayAnim pa, float movespeed, ActionMode am)
{
	if (m_playingAnim != pa)		//�A�C�h����������A���肩�瑁�������̈ړ����@���ω�����Ƃ����ɓ���B
	{
		float ntime = 0.f;
		float animLug = m_animLugBase;
		if (m_playingAnim != m_idolNum)
		{
			//���ꂩ��Đ�����A�j���[�V�����̍Đ��J�n���Ԃ��v�Z�B
			auto& anm = m_chara->GetAnimation();
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
		m_isStartJustFoot = false;
		ChangePlayingAnim(pa);
		m_chara->SetAllIKRub(0.0f);
		//m_chara->SetMoveFunc(m_noneMF);
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
		auto& anm = m_chara->GetAnimation();
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

		m_chara->PlayAnim(m_idolNum, m_animLug_2idle, AM_None);
		m_isWalk = false;
		m_chara->SetAllIKRub(0.0f);
		m_chara->SetIKOffset(ue::CVector3::Zero());
		m_time = m_animLug_2idle;

		
		//m_chara->SetMoveFunc(m_noneMF);
		m_isJustedEnd = false;
		m_isJustedStart = false;
		//m_justTime = 0.f;
		//m_isStartJustFoot = false;
	}
	auto move = m_moved * ((m_time) / m_animLug_2idle);
	//m_chara->SetIKOffset(move * delTime);
	m_chara->SetMove(move);
	m_time -= delTime;
}

void CharacterMoveMotion::JustFoot(float delTime)
{
	if (!m_isStartJustFoot)//�������̕������ɐ�����
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
	case JF_footL://��������X�^�[�g
		startBone = m_footL;
		endBone = m_footR;
		break;
	case JF_footR://�E������X�^�[�g
		startBone = m_footR;
		endBone = m_footL;
		break;
	}

	if (!m_isJustedStart)//�ŏ��ɏo�����̏����B
	{
		auto move = startBone->GetMove().Length();
		if (m_time > 0.2f || move < 10.f)
		{
			m_isJustedStart = true;
			m_chara->SetIKSpeed(m_justFoot_DownIKSpeed, startBone);
			m_time = 0.f;
		}
		else if (m_time < 0.1f)
		{
			auto up = m_chara->GetDir() * -1.f;

			up.y += m_justFoot_OffsetY;
			up.Normalize();
			m_chara->SetIKOffset(up * m_justFoot_Scale, startBone);
			m_chara->SetIKSpeed(m_justFoot_UpIKSpeed, startBone);
		}
		m_time += delTime;
	}
	else if (!m_isJustedEnd)//�Ō�ɏo�����̏����B
	{
		auto move = endBone->GetMove().Length();
		if (m_time > 0.1f || move < 10.f)
		{
			m_isJustedEnd = true;
			m_chara->SetIKSpeed(m_justFoot_DownIKSpeed, endBone);
		}
		auto up = m_chara->GetDir() * -1.f;
		up.y += m_justFoot_OffsetY;
		up.Normalize();
		m_chara->SetIKOffset(up * m_justFoot_Scale, endBone);
		m_chara->SetIKSpeed(m_justFoot_UpIKSpeed, endBone);
		m_time += delTime;
	}
}

void CharacterMoveMotion::ChangePlayingAnim(PlayAnim pa)
{
	m_oldSpeed = m_oldSpeedBuff;
	if (m_oldPlayAnim != m_idolNum)
		m_oldSpeed = m_oldSpeedBuff;
	m_oldPlayAnim = m_playingAnim;
	m_playingAnim = pa;
}
