#include "stdafx.h"
#include "PlayerMotion.h"
#include "Player.h"
#include "Character/Character.h"
#include "Character/CharacterConst.h"

void PlayerMotion::Init(Player* player, Character* chara, ue::Camera* cam, ue::AnimationClip* anim, ue::Pad* pad)
{
	m_player = player;
	m_chara = chara;
	m_camera = cam;
	m_anim = anim;
	m_pad = pad;

	//auto kneeL = m_chara->FindBone(L"Bone_L.002", BK_None, true, 1, 1.f);
	//auto kneeR = m_chara->FindBone(L"Bone_R.002", BK_None, true, 1, 1.f);
	m_footL = m_chara->FindBone(L"Bone_L.003", BK_FootL, true, 3, 1.f);
	m_footR = m_chara->FindBone(L"Bone_R.003", BK_FootR, true, 3, 1.f);
	auto footIKL = m_chara->GetIK(m_footL);
	auto footIKR = m_chara->GetIK(m_footR);
	footIKL->SetRubbing(0.f);
	footIKR->SetRubbing(0.f);
	
	auto handL = m_chara->FindBone(L"Bone.003_L.004", BK_HandL, true, 3, 1.f);
	auto handR = m_chara->FindBone(L"Bone.003_R.004", BK_HandR, true, 3, 1.f);
	m_noneMF = [&](auto& pos) {return;};
	m_noneRF = [&](auto& rote) {return;};

	//m_chara->SetMoveFunc(m_noneMF);
	//m_chara->SetRotateFunc(m_noneRF);
	auto& anime = m_chara->GetAnimation();
	float v = 5.f;
	anime.SetIKGravity(9.8f*v, m_footL);
	anime.SetIKGravity(9.8f*v, m_footR);

	m_charaMove.Init(m_chara, m_anim);
	m_charaMove.InitBone(m_footL, m_footR,false);

	m_charaRotate.Init(m_chara,CharacterRotateMotion::RM_Lerp,15.f);
	m_chara->Init_JustFoot(2.f, 4.f, 30.4f, 35.5f);

	//m_playerClimb.Init(m_chara, m_chara->GetFootLIK(), m_chara->GetFootRIK(), m_chara->GetIK(handL), m_chara->GetIK(handR));
	//m_playerClimb.InitClimbSpec({ 0.5f,0.25f,0.1f,3.f,0.5f,{-7.55f,1.25f,-3},{-5.55f,3.25f,5} });
	//m_playerClimb.StartClimb();
}


void PlayerMotion::Update()
{
	ue::CVector2 stick;
	stick.x = m_pad->GetLStickXF();
	stick.y = m_pad->GetLStickYF();
	float deltime = ue::gameTime()->GetDeltaTime();

	if (!m_playerClimb.IsClimb())
	{
		if (stick.Length() > 0.0001f)
		{
			auto f = m_camera->GetForward() * stick.y;
			auto r = m_camera->GetRight() * stick.x;
			auto move = f + r;
			m_charaRotate.NextRotate(move, 0);
		}

		if (fabsf(stick.x) > 0.01f || fabsf(stick.y) > 0.01f)	//stick入力がある。つまり移動。
		{
			if (m_pad->IsPress(ue::enButtonB))		//ダッシュ
			{
				m_charaMove.NextPlayAnim(PA_dush, m_dushSpeed, AM_None);
			}
			else if (stick.Length() >= 0.5f)				//早歩き
			{
				m_charaMove.NextPlayAnim(PA_walkFast, m_walkFastSpeed, AM_None);
			}
			else														//歩き
			{
				m_charaMove.NextPlayAnim(PA_walk, m_walkSpeed, AM_Move);
			}
		}
		else
		{
			m_charaMove.NextPlayAnim(PA_idol, 0, AM_None);
		}

		UpdateMotion();
	}
	else
	{
		m_playerClimb.UpdateInput(stick);
		m_playerClimb.Update();
	}
	return;
}

void PlayerMotion::UpdateMotion()
{
	m_charaRotate.Update();
	m_charaMove.Update();
}

void PlayerMotion::ToIdole()
{
	m_charaMove.NextPlayAnim(PA_idol, 0, AM_None);
}
