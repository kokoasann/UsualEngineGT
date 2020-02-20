#include "stdafx.h"
#include "CharacterRotateMotion.h"
#include "Character.h"

void CharacterRotateMotion::Release()
{
	m_chara = nullptr;

	m_actionMode = AM_None;
	m_rotateMode = RM_None;
	m_playingAnim = 0;

	m_isPlayAnim = false;
	m_isFirst = true;
	m_isComplete = false;
	m_total = 0.f;
	m_maxRad = 0.f;
	m_speed = ue::CMath::DegToRad(5.f);
	m_threshold = 0.0001f;
	m_dir = ue::CVector3::AxisZ();
	m_oldDir = m_dir;
}

void CharacterRotateMotion::OnDestroy()
{
	Release();
}

void CharacterRotateMotion::Update()
{
	if (m_isComplete)
		return;

	if (m_isPlayAnim && m_isFirst)
	{
		if (RM_UseIK)
			m_actionMode = AM_Rotate;
		else
			m_actionMode = AM_None;
		m_chara->PlayAnim(m_playingAnim, 1, 0.f, m_actionMode);
		m_isFirst = false;
	}

	switch (m_rotateMode)
	{
	case RM_Direct:
		Update_Direct();
		break;
	case RM_Lerp:
		Update_Lerp();
		break;
	case RM_UseIK:
		Update_UseIK();
		break;
	}
}

void CharacterRotateMotion::Update_Direct()
{
	float t = atan2(m_dir.x, m_dir.z);
	//float t = ue::CVector3::AxisZ().Dot(m_dir);
	ue::CQuaternion rot;
	rot.SetRotation(ue::CVector3::AxisY(), t);
	m_chara->SetRotation(rot);
	//m_rotateMode = RM_None;
	Complete();
}

void CharacterRotateMotion::Update_Lerp()
{
	
	ue::CQuaternion add;
	ue::CVector3 axis;
	axis.Cross(m_oldDir, m_dir);

	/*add.SetRotation(ue::CVector3::Up(), m_maxRad <= ue::CMath::PI ? m_maxRad : (m_maxRad - ue::CMath::PI)*-1.f);
	m_chara->SetRotate(add);
	Complete();
	return;*/

	if (m_total < fabsf(m_maxRad))
	{
		float len = fabsf(m_maxRad) - m_total;
		if (len >= m_speed)
		{
			add.SetRotation(ue::CVector3::Up(), m_maxRad >= 0.f ? m_speed : -m_speed);
			m_total += fabsf(m_speed);
		}
		else
		{
			add.SetRotation(ue::CVector3::Up(), m_maxRad >= 0.f ? len : -len);
			m_total += len;
		}
		m_chara->SetRotate(add);
	}
	else
	{
		//m_rotateMode = RM_None;
		Complete();
	}
}

void CharacterRotateMotion::Update_UseIK()
{
	auto t = m_chara->GetDir().Dot(m_dir);
	t = acosf(t);
	if (t < ue::CMath::DegToRad(0.001f))
	{
		//m_rotateMode = RM_None;
		Complete();
	}
}

void CharacterRotateMotion::Complete()
{
	if (!m_isComplete)
	{
		if(m_isPlayAnim)
			m_chara->PlayAnim(CHARA_IDOL_NUM, 1, 0);
		m_isComplete = true;
	}
}

void CharacterRotateMotion::NextRotate(const ue::CVector3& dir, PlayAnim pa, bool isPlayAnim)
{
	m_oldDir = m_chara->GetDir();
	m_dir = dir;
	m_dir.y = 0;
	m_dir.Normalize();
	float t = m_oldDir.Dot(m_dir);
	
	m_maxRad = acosf(fabsf(t)<1.f?t:t/fabsf(t));
	
	ue::CVector3 vv;
	vv.Cross(m_oldDir, m_dir);
	if (vv.y < 0)
	{
		m_maxRad *= -1.f;
	}
	
	m_playingAnim = pa;

	m_total = 0.f;
	m_isFirst = true;
	m_isPlayAnim = isPlayAnim;
	m_isComplete = false;
}

void CharacterRotateMotion::BreakRotate()
{
	Complete();
}
