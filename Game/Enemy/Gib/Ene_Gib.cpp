#include "stdafx.h"
#include "Ene_Gib.h"

Ene_Gib::Ene_Gib()
{
	ue::SkinModelRender* model = ue::NewGO<ue::SkinModelRender>(0);
	m_animClip[0].Load(L"Assets/model/gib/gib.idol.tka");
	m_animClip[0].SetLoopFlag(true);
	m_animClip[1].Load(L"Assets/model/gib/gib.walk1.tka");
	m_animClip[1].SetLoopFlag(true);

	model->Init(L"Assets/model/gib.bone.cmo", m_animClip, 2, ue::enFbxUpAxisY);
	
	m_chara.Init(model, 20, 50, {0,0,0});
	auto footR = m_chara.FindBone(L"IK_Bone.007_R.003", Character::BK_FootR, true, 3, 60);
	auto footL = m_chara.FindBone(L"IK_Bone.007_L.003", Character::BK_FootL, true, 3, 60);
	m_chara.FindBone(L"IK_Bone.007_R.004", Character::BK_None, true, 2, 60);
	m_chara.FindBone(L"IK_Bone.007_L.004", Character::BK_None, true, 2, 60);
	m_chara.PlayAnim(1,1.0f, Character::AM_Move);
}

Ene_Gib::~Ene_Gib()
{

}

void Ene_Gib::OnDestroy()
{

}

bool Ene_Gib::Start()
{
	return false;
}

void Ene_Gib::Update()
{
}
