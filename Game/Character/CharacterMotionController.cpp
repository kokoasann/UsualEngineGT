#include "stdafx.h"
#include "CharacterMotionController.h"

void CharacterMotionController::Release()
{
	m_chara = nullptr;
	m_initCount = 0;
}

void CharacterMotionController::OnDestroy()
{
	Release();
}

void CharacterMotionController::Init(Character* chara, ue::AnimationClip* animClip)
{
	m_moveMotion.Init(chara, animClip);
	m_chara = chara;
	m_initCount++;
}

void CharacterMotionController::InitMove(ue::Bone* footL, ue::Bone* footR)
{
	m_moveMotion.InitBone(footL, footR);
	m_initCount++;
}

void CharacterMotionController::InitRotate(CharacterRotateMotion::RotateMode rotateMode, float speedDeg)
{
	m_rotateMotion.Init(m_chara, rotateMode, speedDeg);
	m_initCount++;
}

bool CharacterMotionController::Start()
{
	return true;
}

void CharacterMotionController::Update()
{
	if (m_initCount < 3)
	{
		MessageBox(NULL, "‰Šú‰»‚Å‚«‚Ä‚È‚¢‚Å‚·B", "init miss", MB_OK);
		std::abort();
	}
	m_moveMotion.Update();
	m_rotateMotion.Update();
}

void CharacterMotionController::PlayMove(unsigned int playNum, float speed, ActionMode actionMode)
{
	m_playMotion = PM_Move;
	if (m_isPlatAnimRotate)
		m_rotateMotion.BreakRotate();
	m_moveMotion.NextPlayAnim(playNum, speed, actionMode);
	
}

void CharacterMotionController::PlayRotate(const ue::CVector3& dir, unsigned int playNum, bool isPlayAnim)
{
	m_playMotion = PM_Rotate;
	if (isPlayAnim)
		m_moveMotion.BreakAnim();
	m_rotateMotion.NextRotate(dir, playNum, isPlayAnim);
	m_isPlatAnimRotate = isPlayAnim;
	
}

void CharacterMotionController::PlayIdol()
{
	switch (m_playMotion)
	{
	case PM_Move:
		m_moveMotion.NextPlayAnim(m_idolNum, 0, AM_None);
		break;
	case PM_Rotate:
		m_rotateMotion.BreakRotate();
		break;
	}
}
