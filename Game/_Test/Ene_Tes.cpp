#include "stdafx.h"
#include "Ene_Tes.h"

Ene_GibTes::Ene_GibTes()
{
	ue::SkinModelRender* model = ue::NewGO<ue::SkinModelRender>(0);
	m_animClip[GA_idol].Load(L"Assets/model/gib_tes/gib.bone.neck_idol.tka");
	m_animClip[GA_idol].SetLoopFlag(true);

	model->Init(L"Assets/model/gib.bone.neck.cmo", 0, 0, ue::enFbxUpAxisY);
	model->SetSca({ 100.f,100.f, 100.f });
	
	m_chara.Init(model, 20, 50, {0,-30,0});
	m_chara.SetMoveFunc([&](auto& p) {return; });
	m_chara.SetRotateFunc([&](auto& p) {return; });
	//auto footR = m_chara.FindBone(L"IK_Bone.007_R.003", Character::BK_FootR, true, 3, 60);
	//auto footL = m_chara.FindBone(L"IK_Bone.007_L.003", Character::BK_FootL, true, 3, 60);
	//m_chara.SetBone(footR->GetChildren()[0], Character::BK_None, true, 2, 10);
	//m_chara.SetBone(footL->GetChildren()[0], Character::BK_None, true, 2, 10);
	//m_chara.SetBone(footR->GetParent()->GetParent()->GetParent(), Character::BK_WaistR);
	//m_chara.SetBone(footL->GetParent()->GetParent()->GetParent(), Character::BK_WaistL);
	m_neck = m_chara.FindBone(L"Head", BK_None, true, 3, 10);


	AddComponent(&m_chara);
}

Ene_GibTes::~Ene_GibTes()
{

}

void Ene_GibTes::OnDestroy()
{

}

bool Ene_GibTes::Start()
{
	return true;
}

void Ene_GibTes::Update()
{


	UpdateComponent();
}
