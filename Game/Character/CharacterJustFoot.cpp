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

	const auto& pos = m_chara->GetPos();
	ue::Bone* currentBone = nullptr;
	bool* isJusted = nullptr;
	if (!m_isJustedStart)//
	{
		currentBone = startBone;
		isJusted = &m_isJustedStart;
	}
	else if(!m_isJustedEnd)
	{
		currentBone = endBone;
		isJusted = &m_isJustedEnd;
	}
	else
	{
		m_isStart = false;
		return;
	}
	

	ue::IK* ik = m_chara->GetIK(currentBone);
	if (!m_isUped)
	{
		if (ik->GetTarget().y >= m_justFoot_Scale)
		{
			m_isUped = true;
		}
		auto up = m_chara->GetDir() * -1.f;
		up.y += m_justFoot_OffsetY;
		up.Normalize();
		up *= delTime * m_justFoot_UpIKSpeed;
		m_nowUP += up;
		ik->SetNextTarget(ik->GetTarget() + up);
		//m_chara->SetIKOffset(m_nowUP, startBone);+

	}
	else
	{

		if (ik != nullptr)
		{
			const auto& tar = ik->GetOldNewTarget();
			auto mpos = m_chara->GetPos();
			auto pos = currentBone->GetWorldMatrix().GetTranslation();
			//auto dir = pos - tar;
			
			auto len = mpos - pos;
			auto dir = len;
			dir.Normalize();
			auto move = dir * m_justFoot_DownIKSpeed * delTime;
			
			if (mpos.y > (ik->GetTarget() + move).y)
			{
				float y = move.y / len.y;
				dir = move * y;
			}
			else
			{
				dir = move;
			}
			//m_chara->SetIKOffset(dir,endBone);
			ik->SetNextTarget(ik->GetTarget() + dir);
			if (currentBone->IsONGround())
			{
				m_isUped = false;
				m_nowUP = ue::CVector3::Zero();
				*isJusted = true;
				m_chara->SetIKSpeed(1.f, currentBone);
			}
		}
	}
	
	//if (!m_isJustedStart)//最初に出す足の処理。
	//{
	//	
	//}
	//else if (!m_isJustedEnd)//最後に出す足の処理。
	//{
	//	ue::IK* ik = m_chara->GetIK(endBone);

	//	if (!m_isUped)
	//	{
	//		if (ik->GetTarget().y >= m_justFoot_Scale)
	//		{
	//			m_isUped = true;
	//		}
	//		auto up = m_chara->GetDir() * -1.f;
	//		up.y += m_justFoot_OffsetY;
	//		up.Normalize();
	//		up *= delTime * m_justFoot_UpIKSpeed;
	//		m_nowUP += up;
	//		ik->SetNextTarget(ik->GetTarget() + up);
	//	}
	//	else
	//	{
	//		if (ik != nullptr)
	//		{
	//			const auto& tar = ik->GetOldNewTarget();
	//			auto pos = endBone->GetWorldMatrix().GetTranslation();
	//			auto dir = pos - tar;
	//			dir.Normalize();
	//			auto move = dir * m_justFoot_DownIKSpeed * delTime;
	//			auto mpos = m_chara->GetPos();
	//			if (mpos.y > (ik->GetTarget() + move).y)
	//			{
	//				float y = move.y / (tar + mpos).y;
	//				dir = move * y;
	//			}
	//			else
	//			{
	//				dir = move;
	//			}
	//			//m_chara->SetIKOffset(dir,endBone);
	//			ik->SetNextTarget(ik->GetTarget() + dir);
	//			//m_chara->SetIKSpeed(0.2f, endBone);
	//			if (endBone->IsONGround())
	//			{
	//				m_isUped = false;
	//				m_nowUP = ue::CVector3::Zero();
	//				m_isJustedEnd = true;
	//				m_chara->SetIKSpeed(1.f, endBone);
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	//if (m_time > (1.f - m_justFoot_DownIKSpeed))
	//	{
	//		m_isStart = false;
	//	}
	//}
	m_time += delTime;
}

void CharacterJustFoot::Start_JustFoot()
{
	m_isStart = true;
	//m_chara->SetIKOffset(ue::CVector3::Zero());
	m_isStartJustFoot = false;
	m_isJustedEnd = false;
	m_isJustedStart = false;

	//if (!m_isStartJustFoot)//遠い足の方から先に整える
	{
		bool onGroundL = m_footL->IsONGround(), onGroundR = m_footR->IsONGround();
		if (!onGroundL)
		{
			m_startJustFoot = JF_footL;
		}
		else if (!onGroundR)
		{
			m_startJustFoot = JF_footR;
		}
		else
		{
			m_chara->SetIKOffset(ue::CVector3::Zero());
			auto moveL = m_footL->GetMove().Length();
			auto moveR = m_footR->GetMove().Length();

			if (moveL > moveR)
				m_startJustFoot = JF_footL;
			else
				m_startJustFoot = JF_footR;
		}
		//m_isStartJustFoot = true;
		//m_chara->SetIKRub(0.0f);
		m_time = 0.f;
	}
	
}
