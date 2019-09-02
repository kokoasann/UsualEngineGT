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
	animclip[0].Load(L"Assets/animData/run.tka");
	animclip[0].SetLoopFlag(true);

	ue::CQuaternion rot;
	cam = &ue::usualEngine()->GetMainCamera();
	p1 = ue::NewGO<ue::SkinModelRender>(0);
	p1->Init(L"Assets/model/unityChan.cmo");
	p1->SetPos({ 30,0,50 });

	p2 = ue::NewGO<ue::SkinModelRender>(0);
	p2->Init(L"Assets/model/unityChan.cmo",animclip,1);
	p2->SetPos({ -20,0,-50 });
	rot.SetRotationDeg(ue::CVector3::AxisX(), 90);
	p2->SetRot(rot);
	p2->SetIsShadowCaster(true);
	//p2->SetIsShadowReciever(false);

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
	ground->SetBlendMap(L"Assets/sprite/map.dds");
	ground->SetTexture(0, L"Assets/sprite/kusa.dds");
	ground->SetTexture(1, L"Assets/sprite/tuti.dds");
	//ground->SetRot(rot);

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/octagon.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{ 7, 9.2f, 7 });
	ground->SetPos({0,0,-20000 });
	ground->SetIsShadowCaster(true);

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/octagon.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{ 1.5, 2.2f, 1.5 });
	ground->SetPos({ -1500,0,-10000 });
	ground->SetIsShadowCaster(true);

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/octagon.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{ 0.4, 1.2f, 0.4 });
	ground->SetPos({ 700,0,-3000 });
	ground->SetIsShadowCaster(true);

	light = ue::NewGO<ue::LightDirection>(0);
	light->SetDir({ -0.3f,-1,-0.2f });

	campos = { 0,250,200 };
	//campos = { 0,3000,2000 };
	cam->SetPosition(campos);
	//cam->SetTarget({ 0,2000,-1000 });
	cam->Update();
	return true;
}

void Game::Update()
{
	//return;
	ue::CQuaternion add = ue::CQuaternion::Identity();
	//add.SetRotationDeg(ue::CVector3::AxisY(), 0.5f);
	//rot.Multiply(add);
	//ue::CVector3 p = campos;
	//rot.Multiply(p);

	//auto r = ground->GetRot();
	////r.Multiply(add);
	//add.SetRotationDeg(ue::CVector3::AxisZ(), 0.7f);
	//r.Multiply(add);
	//ground->SetRot(r);
	auto camtar = cam->GetTarget();
	
	static ue::CVector3 ofs = { 0,0,-200 };
	auto pad = ue::GamePad(0);
	float speed = 30.f;
	campos += cam->GetForward() * speed * pad.GetLStickYF();
	campos += cam->GetRight() * speed * pad.GetLStickXF();


	float yx = pad.GetRStickXF();
	float yy = pad.GetRStickYF();
	speed = 5.f;
	ue::CQuaternion r = ue::CQuaternion::Identity();
	add.SetRotationDeg(ue::CVector3::Up(), speed * yx);
	//camrot.Multiply(add);
	add.Multiply(ofs);

	ue::CVector3 axix = ue::CVector3::Zero();
	axix.Cross(ofs, ue::CVector3::AxisY());
	axix.Normalize();
	r.SetRotationDeg(axix, speed * yy);
	add.Multiply(r);
	add.Multiply(ofs);

	camtar = campos + ofs;


	cam->SetPosition(campos);
	cam->SetTarget(camtar);
	cam->Update();
}
