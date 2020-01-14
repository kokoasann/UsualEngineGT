#include "stdafx.h"
#include "CharacterJustFoot.h"
#include "Character.h"

void CharacterJustFoot::Release()
{
	m_chara = nullptr;
	m_footL = nullptr;
	m_footR = nullptr;
}

void CharacterJustFoot::OnDestroy()
{
	Release();
}

void CharacterJustFoot::Update()
{
	if (!m_isStart)
		return;
	float dtime = ue::gameTime()->GetDeltaTime();
	Update_JustFoot(dtime);
}

void CharacterJustFoot::Update_JustFoot(float delTime)
{
	if (!m_isStartJustFoot)//遠い足の方から先に整える
	{
		m_chara->SetIKOffset(ue::CVector3::Zero());
		auto moveL = m_footL->GetMove().Length();
		auto moveR = m_footR->GetMove().Length();

		if (moveL > moveR)
			m_startJustFoot = JF_footL;
		else
			m_startJustFoot = JF_footR;
		m_isStartJustFoot = true;
		m_chara->SetIKRub(0.0f);
		m_time = 0.f;
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
	}
	else if (!m_isJustedEnd)//最後に出す足の処理。
	{
		auto move = endBone->GetMove().Length();
		if (m_time > 0.1f || move < 10.f)
		{
			m_isJustedEnd = true;
			m_chara->SetIKSpeed(m_justFoot_DownIKSpeed, endBone);
			m_time = 0.f;
			//m_isStart = false;
		}
		auto up = m_chara->GetDir() * -1.f;
		up.y += m_justFoot_OffsetY;
		up.Normalize();
		m_chara->SetIKOffset(up * m_justFoot_Scale, endBone);
		m_chara->SetIKSpeed(m_justFoot_UpIKSpeed, endBone);
	}
	else
	{
		if (m_time > (1.f - m_justFoot_DownIKSpeed))
		{
			m_isStart = false;
		}
	}
	m_time += delTime;
}
