#include "stdafx.h"
#include "PlayerClimb.h"
#include "../Character/Character.h"

struct SweepResultClimb :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;
	ue::CVector3 hitPos = ue::CVector3::Zero();
	ue::CVector3 hitNormal = ue::CVector3::Zero();
	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		hitPos.Set(convexResult.m_hitPointLocal);
		hitNormal.Set(convexResult.m_hitNormalLocal);
		return 0.0f;
	}
};

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
	
	m_collider.Create(footL->GetCollisionRadius());
	
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
	memcpy(&m_climbSpec, &spec, sizeof(m_climbSpec));
	//m_climbSpec = spec;
}

void PlayerClimb::Update()
{
	UpdatePairLimbs(m_footLimbs);
	UpdatePairLimbs(m_handLimbs);
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
		limb.nowStep = Step_UP;
		break;
	case Step_Down:
		DelLS(limb.limbState);
		limb.limbState = NewLS<LimbState_Down>();
		limb.nowStep = Step_Down;
		break;
	case Step_Move:
		DelLS(limb.limbState);
		limb.limbState = NewLS<LimbState_Move>();
		limb.nowStep = Step_Move;
		break;
	case Step_Stop:
		DelLS(limb.limbState);
		limb.limbState = NewLS<LimbState_Stop>();
		limb.nowStep = Step_Stop;
		break;
	}
	if (step != Step_None)
	{
		
	}
}

void PlayerClimb::UpdatePairLimbs(PairLimbs& pair)
{
	UpdateLimb(pair.leftLimb);
	UpdateLimb(pair.rightLimb);
	if (!pair.isCoolTime)
	{
		if (pair.leftLimb.nowStep == Step_Stop && pair.rightLimb.nowStep == Step_Stop) //左右ともストップステップになったのでクールタイム開始。
		{
			pair.isCoolTime = true;
			pair.timer = 0.0f;
		}
	}

	if (pair.isCoolTime)
	{
		if (m_climbSpec.changeLug <= pair.timer)
		{
			switch (pair.moveLimb)
			{
			case PairLimbs::Pair_Right:
				pair.moveLimb = PairLimbs::Pair_Left;
				DelLS(pair.leftLimb.limbState);
				pair.leftLimb.limbState = NewLS<LimbState_Up>();
				break;
			case PairLimbs::Pair_Left:
				pair.moveLimb = PairLimbs::Pair_Right;
				DelLS(pair.rightLimb.limbState);
				pair.rightLimb.limbState = NewLS<LimbState_Up>();
				break;
			}

		}
		pair.timer += ue::gameTime()->GetDeltaTime();
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

//手(足)を壁から離して移動ステップにチェンジ
PlayerClimb::LimbStep PlayerClimb::LimbState_Up::Update(const PlayerClimb& body, ue::IK* ik)
{
	auto charaDir = body.m_chara->GetDir();
	ue::CVector3 move;
	//目の前の壁の法線を調べて動かすベクトルを決める
	{
		auto pos = ik->GetTarget();
		auto tar = pos + charaDir * (body.m_climbSpec.forwardLen * 10.f);
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		start.setOrigin(btVector3(pos.x, pos.y, pos.z));
		end.setOrigin(btVector3(tar.x, tar.y, tar.z));

		SweepResultClimb sr;
		ue::Physics().ConvexSweepTest((const btConvexShape*)body.m_collider.GetBody(), start, end, sr);
		if (sr.isHit)
		{
			move = sr.hitNormal * body.m_climbSpec.speed;
		}
		else
		{
			move = charaDir * -1.f * body.m_climbSpec.speed;
		}
	}

	m_upLen += move.Length();
	auto target = ik->GetTarget();
	target += move;
	ik->SetNextTarget(target);

	if (body.m_climbSpec.forwardLen <= m_upLen && !ik->IsHit())
		return Step_Move;
	return Step_None;
}

//手(足)を壁に設置してストップステップにチェンジ
PlayerClimb::LimbStep PlayerClimb::LimbState_Down::Update(const PlayerClimb& body, ue::IK* ik)
{
	auto charaDir = body.m_chara->GetDir();

	ue::CVector3 move;
	//目の前の壁の法線を調べて動かすベクトルを決める
	{
		auto pos = ik->GetTarget();
		auto tar = pos + charaDir * (body.m_climbSpec.forwardLen * 10.f);
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		start.setOrigin(btVector3(pos.x, pos.y, pos.z));
		end.setOrigin(btVector3(tar.x, tar.y, tar.z));

		SweepResultClimb sr;
		ue::Physics().ConvexSweepTest((const btConvexShape*)body.m_collider.GetBody(), start, end, sr);
		if (sr.isHit)
		{
			move = sr.hitNormal * -1.0f * body.m_climbSpec.speed;
		}
		else
		{
			move = charaDir * body.m_climbSpec.speed;
		}
	}

	m_downLen += move.Length();
	auto target = ik->GetTarget();
	target += move;
	ik->SetNextTarget(target);

	if (body.m_climbSpec.forwardLen <= m_downLen && ik->IsHit())
		return Step_Stop;
	return Step_None;
}

PlayerClimb::LimbStep PlayerClimb::LimbState_Move::Update(const PlayerClimb& body, ue::IK* ik)
{
	auto charaDir = body.m_chara->GetDir();

	ue::CVector3 move;
	//目の前の壁の法線を調べて動かすベクトルを決める
	{
		auto pos = ik->GetTarget();
		auto tar = pos + charaDir * (body.m_climbSpec.forwardLen * 10.f);
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		start.setOrigin(btVector3(pos.x, pos.y, pos.z));
		end.setOrigin(btVector3(tar.x, tar.y, tar.z));

		SweepResultClimb sr;
		ue::Physics().ConvexSweepTest((const btConvexShape*)body.m_collider.GetBody(), start, end, sr);
		if (sr.isHit)
		{
			move = sr.hitNormal;
		}
		else
		{
			move = charaDir * -1.0f;
		}
	}
	ue::CVector3 X,Z=move;
	X.Cross(Z,ue::CVector3::AxisY());
	move.Cross(Z, X);

	//パッドの入力に合わせてmoveを回転させる
	auto inp = body.m_moveDir;
	inp.Normalize();
	float dot = inp.Dot(ue::CVector2(0.0f, 1.0f));
	float rad = acos(fabsf(dot)<=1.0f?dot:dot/fabsf(dot));	//dotの結果が1より大きくないか調べて、大きい場合は符号を残して1にする

	ue::CVector2 crossRes;
	crossRes.Cross(ue::CVector2(0.f, 1.f), inp);
	if (crossRes.x <= 0)	//(0,1)を基準にゲームパッドが左右どちらに傾いているか調べる
	{
		//左に傾いている場合は-1を掛けて逆回転させる
		rad *= -1.f;
	}

	ue::CQuaternion rot;
	rot.SetRotation(Z, rad);
	rot.Multiply(move);

	move *= body.m_climbSpec.speed;
	
	auto target = ik->GetTarget();
	target += move;

	if (body.m_climbSpec.upLen < (body.m_chara->GetPos() - ik->GetTarget()).Length())	// 伸ばし終わったら手(足)を下す
		return Step_Down;
	return Step_None;
}

PlayerClimb::LimbStep PlayerClimb::LimbState_Stop::Update(const PlayerClimb& body, ue::IK* ik)
{
	return Step_None;
}