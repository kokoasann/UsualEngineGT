#include "stdafx.h"
#include "PlayerClimb.h"
#include "../Character/Character.h"

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

void PlayerClimb::Init(Character* chara,ue::IK* footL, ue::IK* footR, ue::IK* handL, ue::IK* handR)
{
	m_chara = chara;
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

void PlayerClimb::InitClimbSpec(const ClimbSpec& spec)
{
	m_climbSpec = spec;
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
	m_footLIK->SetNextTarget(m_chara->GetPos() + m_climbSpec.startFootPos);
	ue::CVector2 inv = m_climbSpec.startFootPos;
	inv.x *= -1.f;
	m_footRIK->SetNextTarget(m_chara->GetPos() + inv);


	m_handLIK->SetNextTarget(m_chara->GetPos() + m_climbSpec.startHandPos);
	inv = m_climbSpec.startHandPos;
	inv.x *= -1.f;
	m_handRIK->SetNextTarget(m_chara->GetPos() + inv);


}

void PlayerClimb::BreakClimb()
{
}

void PlayerClimb::StopClimb()
{
}



/// <summary>
/// 
///		こっからは LimbState 関係。
/// 
/// </summary>



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


/// <summary>
/// 
///		limbState 更新系。
/// 
/// </summary>

void PlayerClimb::LimbState_Move::Update(const PlayerClimb& body, ue::IK* ik)
{
	
}

void PlayerClimb::LimbState_Stop::Update(const PlayerClimb& body, ue::IK* ik)
{
}