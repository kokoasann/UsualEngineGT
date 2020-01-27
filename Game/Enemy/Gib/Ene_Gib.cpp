#include "stdafx.h"
#include "Ene_Gib.h"
#include "Character/CharacterConst.h"
#include "Physics/CollisionAttr.h"
Ene_Gib::Ene_Gib()
{
	ue::SkinModelRender* model = ue::NewGO<ue::SkinModelRender>(0);
	m_animClip[GA_idol].Load(L"Assets/model/gib/gib.idol.tka");
	m_animClip[GA_idol].SetLoopFlag(true);
	m_animClip[GA_walk].Load(L"Assets/model/gib/gib.walk1.tka");
	m_animClip[GA_walk].SetLoopFlag(true);
	m_animClip[GA_rotate].Load(L"Assets/model/gib/gib.rotation.tka");
	m_animClip[GA_rotate].SetLoopFlag(true);

	model->Init(L"Assets/model/gib.bone.cmo", m_animClip, GA_num, ue::enFbxUpAxisY);
	model->SetIsShadowReciever(true);
	model->SetIsShadowCaster(true);
	//model->Init(L"Assets/model/gib.bone.cmo", 0,0, ue::enFbxUpAxisY);
	model->SetSca({ 100.f,100.f, 100.f });
	
	m_chara.Init(model, 20, 50, {0,-30,0},false);

	auto footR = m_chara.FindBone(L"IK_Bone.007_R.003", BK_FootR, true, 3, 60);	//‘«
	auto footL = m_chara.FindBone(L"IK_Bone.007_L.003", BK_FootL, true, 3, 60);
	//m_chara.SetBone(footR->GetChildren()[0], BK_None, true, 2, 10);					//‘«æ
	//m_chara.SetBone(footL->GetChildren()[0], BK_None, true, 2, 10);
	//m_chara.SetBone(footR, BK_FootR, true, 3, 60);
	//m_chara.SetBone(footL, BK_FootL, true, 3, 60);
	m_chara.SetBone(footR->GetParent()->GetParent()->GetParent(), BK_WaistR);
	m_chara.SetBone(footL->GetParent()->GetParent()->GetParent(), BK_WaistL);
	//m_chara.FindBone(L"Bone.007_R.004", Character::BK_None, true, 2, 10);
	//m_chara.FindBone(L"Bone.007_L.004", Character::BK_None, true, 2, 10);
	ue::Animation& anim = m_chara.GetAnimation();
	float v = 25.5f;
	anim.SetIKGravity(9.8f*v, footR);
	anim.SetIKGravity(9.8f*v, footL);
	anim.SetIKGravity(9.8f * v, footR->GetChildren()[0]);
	anim.SetIKGravity(9.8f * v, footL->GetChildren()[0]);

	m_chara.SetActionMode(AM_None);
	//m_chara.PlayAnim(GA_rotate,1.0f, 0.f,AM_Rotate);
	//m_chara.PlayAnim(GA_walk, 1.0f, 0.f, AM_Move);

	m_chara.SetIKRub(0, footR->GetChildren()[0]);
	m_chara.SetIKRub(0, footL->GetChildren()[0]);
	m_chara.Init_JustFoot(2.f, 500.f, 700.f, 900.f);

	m_charaMove.Init(&m_chara, m_animClip);
	m_charaMove.InitBone(footL, footR);
	//m_charaMove.InitJustFoot(5.f, 10.f, 10.4f, 2.6f);
	m_charaRotate.Init(&m_chara, CharacterRotateMotion::RM_UseIK, 0);

	m_cbc = ue::NewGO<ue::CharacterBoxCollider>(0);
	m_cbc->Init(L"Assets/model/gib/gib.ubc", &model->GetSkinModel().GetSkeleton(), 100.f, [&](const char* tag)
	{
		int ind = ue::enCollisionAttr_NonHitIK|CUI_Monster;
		if (strcmp("head", tag)==0)
		{
			ind |= MT_Head;
		}
		return ind;
	});

	std::function<void()> f = [&]() 
	{
		auto& cam = ue::usualEngine()->GetMainCamera();
		//cam.CameraModeOrth();
		auto v = m_chara.GetPos();
		v.y += 600;
		cam.SetTarget(v);
		v.x += 1600;
		cam.SetPosition(v);
		cam.Update();
	};
	ue::DebugSwitchAddRadio(ue::DebugSwitchNewSwitch('Z', 0, f));
	f = [&]()
	{
		auto& cam = ue::usualEngine()->GetMainCamera();
		
		auto v = m_chara.GetPos();
		v.y += 30;
		cam.SetTarget(v);
		v.y += 80;
		v.x += 600;
		cam.SetPosition(v);
		cam.Update();
	};
	ue::DebugSwitchAddRadio(ue::DebugSwitchNewSwitch('Z', VK_CONTROL, f));

	f = [&]()
	{
		ue::CVector3 zero = ue::CVector3::Zero();
		zero.y += 10;
		m_chara.SetPos(zero);
	};
	ue::DebugSwitchAddCheck(ue::DebugSwitchNewSwitch('X', 0, f, f));
	
	ue::DebugSwitchNewRadioBox("gibMove");
	f = [&]()
	{
		m_chara.PlayAnim(GA_walk, 1.0f, 0.f, AM_Move);
		//m_charaMove.NextPlayAnim(GA_walk, 1, AM_Move);
	};
	ue::DebugSwitchAddRadioBoxButton("gibMove", ue::DebugSwitchNewSwitch('C', 0, f));
	f = [&]()
	{
		m_chara.PlayAnim(GA_rotate, 1.0f, 0.f, AM_Rotate);
	};
	ue::DebugSwitchAddRadioBoxButton("gibMove", ue::DebugSwitchNewSwitch('V', 0, f));
	f = [&]()
	{
		m_chara.PlayAnim(GA_idol, 1.0f, 0.f, AM_None);
		//m_charaMove.NextPlayAnim(GA_idol, 0, AM_None);
	};
	ue::DebugSwitchAddRadioBoxButton("gibMove", ue::DebugSwitchNewSwitch('B', 0, f));
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
