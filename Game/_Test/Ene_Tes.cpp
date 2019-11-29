#include "stdafx.h"
#include "Ene_Tes.h"

Ene_Gib::Ene_Gib()
{
	ue::SkinModelRender* model = ue::NewGO<ue::SkinModelRender>(0);
	m_animClip[GA_idol].Load(L"Assets/model/gib/gib.bone.neck_idol.tka");
	m_animClip[GA_idol].SetLoopFlag(true);

	model->Init(L"Assets/model/gib.bone.neck.cmo", m_animClip, 1, ue::enFbxUpAxisY);
	model->SetSca({ 100.f,100.f, 100.f });
	
	m_chara.Init(model, 20, 50, {0,-30,0});
	auto footR = m_chara.FindBone(L"IK_Bone.007_R.003", Character::BK_FootR, true, 3, 60);
	auto footL = m_chara.FindBone(L"IK_Bone.007_L.003", Character::BK_FootL, true, 3, 60);
	m_chara.SetBone(footR->GetChildren()[0], Character::BK_None, true, 2, 10);
	m_chara.SetBone(footL->GetChildren()[0], Character::BK_None, true, 2, 10);
	m_chara.SetBone(footR->GetParent()->GetParent()->GetParent(), Character::BK_WaistR);
	m_chara.SetBone(footL->GetParent()->GetParent()->GetParent(), Character::BK_WaistL);
	m_neck = m_chara.FindBone(L"Head", Character::BK_None, true, 3, 10);

	m_chara.PlayAnim(GA_rotate,1.0f, 0.f,Character::AM_Rotate);

	AddComponent(&m_chara);
}

Ene_Gib::~Ene_Gib()
{

}

void Ene_Gib::OnDestroy()
{

}

bool Ene_Gib::Start()
{
	return true;
}

void Ene_Gib::Update()
{


	UpdateComponent();
}
