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

	campos = { 200,50,0 };
	cam->SetPosition(campos);
	cam->Update();
	return true;
}

void Game::Update()
{
	ue::CQuaternion add = ue::CQuaternion::Identity();
	add.SetRotationDeg(ue::CVector3::AxisY(), 2);
	rot.Multiply(add);
	ue::CVector3 p = campos;
	rot.Multiply(p);
	
	cam->SetPosition(p);
	cam->Update();
}
