#include "stdafx.h"
#include "CharacterRotateMotion.h"
#include "Character.h"

void CharacterRotateMotion::Update()
{
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
	float t = ue::CVector3::AxisZ().Dot(m_dir);
	ue::CQuaternion rot;
	rot.SetRotation(ue::CVector3::AxisY(), t);
	m_chara->SetRotation(rot);
	m_rotateMode = RM_None;
}

void CharacterRotateMotion::Update_Lerp()
{
	ue::CQuaternion add;
	ue::CVector3 axis;
	axis.Cross(m_oldDir, m_dir);

	if (m_total < m_maxRad)
	{
		float len = m_maxRad - m_total;
		if (len >= m_speed)
		{
			add.SetRotation(axis, m_speed);
			m_total += m_speed;
		}
		else
		{
			add.SetRotation(axis, len);
			m_total += len;
		}
		m_chara->SetRotate(add);
	}
	else
	{
		m_rotateMode = RM_None;
	}
}

void CharacterRotateMotion::Update_UseIK()
{

}
