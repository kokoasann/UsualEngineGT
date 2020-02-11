#include "stdafx.h"
#include "PlayerClimb.h"

PlayerClimb::PlayerClimb()
{
}

PlayerClimb::~PlayerClimb()
{
}

void PlayerClimb::Release()
{
}

void PlayerClimb::OnDestroy()
{
	Release();
}

void PlayerClimb::Init(ue::IK* footL, ue::IK* footR, ue::IK* handL, ue::IK* handR)
{
	m_footLIK = footL;
	m_footRIK = footR;
	m_handLIK = handL;
	m_handRIK = handR;
	
	m_moveFunc = [&](ue::CVector3& pos)
	{
		bool isHitR = m_handRIK->IsHit();
		bool isHitL = m_handLIK->IsHit();

		if (isHitL != isHitR)
		{
			ue::CVector3 move;
			if (isHitL)
				move = m_handLIK->GetMove();
			else if (isHitR)
				move = m_handRIK->GetMove();

			pos += move;
		}
	};
	m_rotateFunc = [&](ue::CQuaternion& rot)
	{

	};
}

void PlayerClimb::Update()
{

}

void PlayerClimb::PostUpdate()
{
}

void PlayerClimb::UpdateInput(const ue::CVector2& dir)
{
	m_moveDir = dir;
	m_moveDir.Normalize();
}

void PlayerClimb::StartClimb()
{

}

void PlayerClimb::BreakClimb()
{
}

void PlayerClimb::StopClimb()
{
}


/// <summary>
/// 
///		Ç±Ç¡Ç©ÇÁÇÕ LimbState ä÷åWÅB
/// 
/// </summary>

void PlayerClimb::LimbState_Stop::Update(ue::IK* ik)
{
}

void* PlayerClimb::LimbState_Move::operator new(size_t size)
{
	void* ptr = nullptr;
	if (TrashBox.size() == 0)
	{
		ptr = malloc(size);
		AllInstance.push_back((LimbState_Move*)ptr);
	}
	else
	{
		ptr = (void*)TrashBox[TrashBox.size() - 1];
		TrashBox.pop_back();
	}
	return ptr;
}

void PlayerClimb::LimbState_Move::Release()
{
	for (auto item : AllInstance)
	{
		free(item);
	}
	AllInstance.clear();
	TrashBox.clear();
}

PlayerClimb::LimbState_Move::~LimbState_Move()
{

}

void PlayerClimb::LimbState_Move::Update(ue::IK* ik)
{
}
