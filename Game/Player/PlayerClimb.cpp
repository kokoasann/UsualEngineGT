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

	//　手
	{
		Limb& RLimb = m_handLimbs.rightLimb;
		Limb& LLimb = m_handLimbs.leftLimb;
		LLimb.ik = m_handLIK;
		LLimb.limbState = NewLS<LimbState_Stop>();
		RLimb.ik = m_handRIK;
		RLimb.limbState = NewLS<LimbState_Stop>();
	}

	//　足
	{
		Limb& RLimb = m_footLimbs.rightLimb;
		Limb& LLimb = m_footLimbs.leftLimb;
		LLimb.ik = m_footLIK;
		LLimb.limbState = NewLS<LimbState_Stop>();
		RLimb.ik = m_footRIK;
		RLimb.limbState = NewLS<LimbState_Stop>();
	}
	
	
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
	UpdateLimbState(m_footLIK, m_footLState);
	UpdateLimbState(m_footRIK, m_footRState);
	UpdateLimbState(m_handLIK, m_handLState);
	UpdateLimbState(m_handRIK, m_handRState);
	
}

void PlayerClimb::PostUpdate()
{
}

void PlayerClimb::UpdateInput(const ue::CVector2& dir)
{
	m_moveLen = dir.Length();
	m_moveDir = dir;
	m_moveDir.Normalize();
}

void PlayerClimb::UpdateLimbState(ue::IK* ik,LimbState*& limbState)
{
	LimbStep step = limbState->Update(*this, ik);
	
	switch (step)
	{
	case Step_None://なんもしない
		break;
	case Step_UP:
		DelLS(limbState);
		limbState = NewLS<LimbState_Up>();
		break;
	case Step_Down:
		DelLS(limbState);
		limbState = NewLS<LimbState_Down>();
		break;
	case Step_Move:
		DelLS(limbState);
		limbState = NewLS<LimbState_Move>();
		break;
	case Step_Stop:
		DelLS(limbState);
		limbState = NewLS<LimbState_Stop>();
		break;
	}
}

void PlayerClimb::UpdateLimb(Limb& limb)
{
	LimbStep step = limb.limbState->Update(*this, limb.ik);

	switch (step)
	{
	case Step_None://なんもしない
		break;
	case Step_UP:
		DelLS(limb.limbState);
		limb.limbState = NewLS<LimbState_Up>();
		break;
	case Step_Down:
		DelLS(limb.limbState);
		limb.limbState = NewLS<LimbState_Down>();
		break;
	case Step_Move:
		DelLS(limb.limbState);
		limb.limbState = NewLS<LimbState_Move>();
		break;
	case Step_Stop:
		DelLS(limb.limbState);
		limb.limbState = NewLS<LimbState_Stop>();
		break;
	}
	if (step != Step_None)
	{
		
	}
}

void PlayerClimb::UpdatePairLimbs(PairLimbs& pair)
{

	if (m_climbSpec.changeLug <= pair.timer)
	{
		switch (pair.moveLimb)
		{
		case PairLimbs::Pair_Right:
			break;
		case PairLimbs::Pair_Left:
			break;
		}
	}
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

std::vector<PlayerClimb::LimbState_Move*>PlayerClimb::LimbState_Move::AllInstance;
std::vector<PlayerClimb::LimbState_Stop*>PlayerClimb::LimbState_Stop::AllInstance;

std::vector<PlayerClimb::LimbState_Move*>PlayerClimb::LimbState_Move::TrashBox;
std::vector<PlayerClimb::LimbState_Stop*>PlayerClimb::LimbState_Stop::TrashBox;


/// <summary>
/// 
///	LimbState　の　Init系。
/// 
/// </summary>
void PlayerClimb::LimbState_Up::Init()
{
}
void PlayerClimb::LimbState_Down::Init()
{
}
void PlayerClimb::LimbState_Move::Init()
{
}
void PlayerClimb::LimbState_Stop::Init()
{
}


/// <summary>
/// 
///		limbState 更新系。
/// 
/// </summary>
PlayerClimb::LimbStep PlayerClimb::LimbState_Up::Update(const PlayerClimb& body, ue::IK* ik)
{
	auto charaDir = body.m_chara->GetDir();
	
	auto move = charaDir * -1.f * body.m_climbSpec.speed;
	m_upLen += move.Length();
	auto target = ik->GetTarget();
	target += move;

	if (body.m_climbSpec.forwardLen <= m_upLen && !ik->IsHit())
		return Step_Move;
	return Step_None;
}

PlayerClimb::LimbStep PlayerClimb::LimbState_Down::Update(const PlayerClimb& body, ue::IK* ik)
{
	auto charaDir = body.m_chara->GetDir();

	auto move = charaDir * body.m_climbSpec.speed;
	m_downLen += move.Length();
	auto target = ik->GetTarget();
	target += move;

	if (body.m_climbSpec.forwardLen <= m_downLen && ik->IsHit())
		return Step_Move;
	return Step_None;
}

PlayerClimb::LimbStep PlayerClimb::LimbState_Move::Update(const PlayerClimb& body, ue::IK* ik)
{


	return Step_None;
}

PlayerClimb::LimbStep PlayerClimb::LimbState_Stop::Update(const PlayerClimb& body, ue::IK* ik)
{
	return Step_None;
}