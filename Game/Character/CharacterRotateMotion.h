#pragma once
#include "CharacterConst.h"

class Character;
class CharacterRotateMotion :public ue::GameObject
{
public:
	CharacterRotateMotion(){}
	~CharacterRotateMotion(){}

	void Update() override;

	void Update_Direct();
	void Update_Lerp();
	void Update_UseIK();

	/// <summary>
	/// dir�̕��Ɍ�������
	/// </summary>
	/// <param name="dir">dir�͂��̉�]������L��������̑��ΓI�Ȃ��̂łȂ��Ă͂Ȃ�Ȃ�</param>
	void NextRotate(const ue::CVector3& dir)
	{
		m_oldDir = m_dir;
		m_dir = dir;
		float t = m_oldDir.Dot(m_dir);
		m_maxRad = acosf(t);
	}
	enum RotateMode
	{
		RM_None,
		RM_Direct,
		RM_Lerp,
		RM_UseIK
	};
private:
	Character* m_chara = nullptr;

	ActionMode m_actionMode = AM_None;
	RotateMode m_rotateMode = RM_None;
	float m_total = 0.f;
	float m_maxRad = 0.f;
	float m_speed = ue::CMath::DegToRad(5.f);
	float m_threshold = 0.0001f;
	ue::CVector3 m_dir = ue::CVector3::AxisZ();
	ue::CVector3 m_oldDir = m_dir;
};