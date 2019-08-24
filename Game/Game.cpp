#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

void Game::OnDestroy()
{
	ue::DeleteGO(p1);
	ue::DeleteGO(p2);
}

bool Game::Start()
{
	cam = &ue::usualEngine()->GetMainCamera();
	p1 = ue::NewGO<ue::SkinModelRender>(0);
	p1->Init(L"Assets/model/unityChan.cmo");
	p1->SetPos({ 30,0,50 });

	p2 = ue::NewGO<ue::SkinModelRender>(0);
	p2->Init(L"Assets/model/unityChan.cmo");
	p2->SetPos({ -20,0,-50 });
	p2->SetIsShadowCaster(true);

	p3 = ue::NewGO<ue::SkinModelRender>(0);
	p3->Init(L"Assets/model/unityChan.cmo");
	p3->SetPos({ 0,-50,1500 });
	p3->SetSca({ 30,0.4f,30 });
	ue::CQuaternion rot;
	rot.SetRotationDeg(ue::CVector3::AxisX(), -90);
	p3->SetRot(rot);
	p3->SetIsShadowCaster(true);

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/dun.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{1, 2, 1});
	ground->SetPos({ 100,0,0 });

	campos = { 10,3500,0 };
	cam->SetPosition(campos);
	cam->Update();
	return true;
}

void Game::Update()
{
	ue::CQuaternion add = ue::CQuaternion::Identity();
	add.SetRotationDeg(ue::CVector3::AxisY(), 0.5f);
	rot.Multiply(add);
	ue::CVector3 p = campos;
	rot.Multiply(p);
	
	//cam->SetPosition(p);
	cam->Update();
}
