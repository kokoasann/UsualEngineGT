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


	if (!m_isJustedStart)//最初に出す足の処理。
	{
		if (!m_isUped)
		{
			auto up = m_chara->GetDir() * -1.f;
			up.y += m_justFoot_OffsetY;
			up.Normalize();
			up *= delTime * m_justFoot_UpIKSpeed;
			m_nowUP += up;
			m_chara->SetIKOffset(m_nowUP, startBone);
			if (m_nowUP.y >= m_justFoot_Scale)
			{
				m_isUped = true;
			}
		}
		else
		{
			auto ik = m_chara->GetIK(startBone);
			const auto& tar = ik->GetOldNewTarget();
			auto pos = startBone->GetWorldMatrix().GetTranslation();
			auto dir = pos - tar;
			dir.Normalize();
			dir *= m_justFoot_DownIKSpeed * delTime;
			//m_chara->SetIKOffset(dir,endBone);
			ik->SetNextTarget(ik->GetTarget() + dir);
			if (startBone->IsONGround())
			{
				m_isUped = false;
				m_nowUP = ue::CVector3::Zero();
				m_isJustedStart = true;
				m_chara->SetIKSpeed(1.f, startBone);
			}

		}
	}
	else if (!m_isJustedEnd)//最後に出す足の処理。
	{
		//auto move = endBone->GetMove().Length();
		//if (m_time > 0.1f || move < 10.f)
		//{
		//	m_isJustedEnd = true;
		//	m_chara->SetIKSpeed(m_justFoot_DownIKSpeed, endBone);
		//	m_time = 0.f;
		//	//m_isStart = false;
		//}
		//auto up = m_chara->GetDir() * -1.f;
		//up.y += m_justFoot_OffsetY;
		//up.Normalize();
		//m_chara->SetIKOffset(up * m_justFoot_Scale, endBone);
		//m_chara->SetIKSpeed(m_justFoot_UpIKSpeed, endBone);

		if (!m_isUped)
		{
			auto up = m_chara->GetDir() * -1.f;
			up.y += m_justFoot_OffsetY;
			up.Normalize();
			up *= delTime * m_justFoot_UpIKSpeed;
			m_nowUP += up;
			m_chara->SetIKOffset(m_nowUP, endBone);
			if (m_nowUP.y >= m_justFoot_Scale)
			{
				m_isUped = true;
			}
		}
		else
		{
			/*auto up = m_chara->GetDir() * -1.f;
			up.y += m_justFoot_OffsetY;
			up.Normalize();
			up *= delTime * m_justFoot_UpIKSpeed;
			m_nowUP -= up;
			m_chara->SetIKOffset(m_nowUP, endBone);*/
			auto ik = m_chara->GetIK(endBone);
			const auto& tar = ik->GetOldNewTarget();
			auto pos = endBone->GetWorldMatrix().GetTranslation();
			auto dir = pos-tar;
			dir.Normalize();
			dir *= m_justFoot_DownIKSpeed*delTime;
			//m_chara->SetIKOffset(dir,endBone);
			ik->SetNextTarget(ik->GetTarget() + dir);
			//m_chara->SetIKSpeed(0.2f, endBone);
			if (endBone->IsONGround())
			{
				m_isUped = false;
				m_nowUP = ue::CVector3::Zero();
				m_isJustedEnd = true;
				m_chara->SetIKSpeed(1.f, endBone);
			}
		}
	}
	else
	{
		//if (m_time > (1.f - m_justFoot_DownIKSpeed))
		{
			m_isStart = false;
		}
	}
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
