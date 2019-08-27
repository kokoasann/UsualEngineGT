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
	ue::CQuaternion rot;
	cam = &ue::usualEngine()->GetMainCamera();
	p1 = ue::NewGO<ue::SkinModelRender>(0);
	p1->Init(L"Assets/model/unityChan.cmo");
	p1->SetPos({ 30,0,50 });

	p2 = ue::NewGO<ue::SkinModelRender>(0);
	p2->Init(L"Assets/model/unityChan.cmo");
	p2->SetPos({ -20,0,-50 });
	p2->SetIsShadowCaster(true);
	p2->SetIsShadowReciever(false);

	/*p3 = ue::NewGO<ue::SkinModelRender>(0);
	p3->Init(L"Assets/model/unityChan.cmo");
	p3->SetPos({ 0,-50,1500 });
	p3->SetSca({ 30,30,0.4f });
	
	rot.SetRotationDeg(ue::CVector3::AxisX(), -90);
	p3->SetRot(rot);
	p3->SetIsShadowCaster(true);*/

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/dun.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{30, 0.1f, 10});
	ground->SetPos({ 0,-100,0 });
	rot.SetRotationDeg(ue::CVector3::AxisZ(), 90);
	//ground->SetRot(rot);

	light = ue::NewGO<ue::LightDirection>(0);
	light->SetDir({ 0,-1,0 });

	campos = { 300,100,0 };
	cam->SetPosition(campos);
	//cam->SetTarget({ 0,0,0 });
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

	auto r = ground->GetRot();
	//r.Multiply(add);
	add.SetRotationDeg(ue::CVector3::AxisZ(), 0.7f);
	r.Multiply(add);
	//ground->SetRot(r);

	cam->SetPosition(p);
	cam->Update();
}
