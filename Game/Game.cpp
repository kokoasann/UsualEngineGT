#include "stdafx.h"
#include "Game.h"
#include "Player/Player.h"
#include "Enemy/Gib/Ene_Gib.h"
#include "_Test/Ene_Tes.h"
#include "Physics/BoxCollider.h"

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
	ue::NewGO<Player>(0);
	ue::NewGO<Ene_Gib>(0);
	//ue::NewGO<Ene_GibTes>(0);

	ue::BoxCollider* bcp = new ue::BoxCollider(); 
	auto& bc = *bcp;
	bcp->Create({ 100,50,100 });
	ue::RigidBodyInfo info;
	info.mass = 0.f;
	info.collider = bcp;
	info.pos = { 500,10,0 };
	ue::CQuaternion r;
	r.SetRotationDeg(ue::CVector3::AxisX(), 10);
	info.rot = r;
	ue::RigidBody* rbp = new ue::RigidBody();
	auto& rb = *rbp;
	rbp->Create(info);
	ue::Physics().AddRigidBody(*rbp);

	cam = &ue::usualEngine()->GetMainCamera();

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/dun.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetIsShadowCaster(true);
	ground->SetIsShadowReciever(true);
	//ground->SetSca(ue::CVector3{30, 0.1f, 10});
	ground->SetSca(ue::CVector3{30, 30.f, 10});
	//ground->SetPos({ 0,-100,0 });
	ground->SetPos({ 0,-100*300,0 });
	rot.SetRotationDeg(ue::CVector3::AxisZ(), 90);
	ground->SetBlendMap(L"Assets/sprite/map.dds");
	ground->SetTexture(0, L"Assets/sprite/kusa.dds");
	ground->SetTexture(1, L"Assets/sprite/tuti.dds");

	pso.CreateMeshObject(ground->GetSkinModel(), ground->GetPos(), ground->GetRot(),ground->GetSca());
	//ground->SetRot(rot);


	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/dun.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetIsShadowCaster(true);
	ground->SetIsShadowReciever(true);
	//ground->SetSca(ue::CVector3{30, 0.1f, 10});
	ground->SetSca(ue::CVector3{ 1, 1, 1 });
	//ground->SetPos({ 0,-100,0 });
	ground->SetPos({ 0,-900,700 });
	ue::CQuaternion rot;
	rot.SetRotationDeg(ue::CVector3::AxisX(), -10);
	ground->SetRot(rot);
	ground->SetBlendMap(L"Assets/sprite/map.dds");
	ground->SetTexture(0, L"Assets/sprite/kusa.dds");
	ground->SetTexture(1, L"Assets/sprite/tuti.dds");
	pso2.CreateMeshObject(ground->GetSkinModel(), ground->GetPos(), ground->GetRot(), ground->GetSca());

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/octagon.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{ 7, 9.2f, 7 });
	ground->SetPos({0,0,-20000 });
	ground->SetIsShadowCaster(true);
	ground->SetIsShadowReciever(true);

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/octagon.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{ 1.5, 2.2f, 1.5 });
	ground->SetPos({ -1500,0,-10000 });
	ground->SetIsShadowCaster(true);
	ground->SetIsShadowReciever(true);

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/octagon.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{ 0.4, 1.2f, 0.4 });
	ground->SetPos({ 700,0,-3000 });
	ground->SetIsShadowCaster(true);
	ground->SetIsShadowReciever(true);

	light = ue::NewGO<ue::LightDirection>(0);
	light->SetDir(ue::CVector3{ -0.3f,-1,-0.2f });
	//light->SetCol(ue::CVector3::One()* 5.f);

	campos = { 400,20,0 };
	//campos = { 0,3000,2000 };
	cam->SetPosition(campos);
	cam->SetTarget({ 0,20,0 });
	cam->Update();
	
	return true;
}

void Game::Update()
{
	auto& pad = ue::GamePad(0);
	//return;

	ue::CQuaternion add = ue::CQuaternion::Identity();
	
	static std::vector<ue::SkinModelRender*> list;

	if (list.size() != 0)
	{
		float speed = 30.f;
		auto npo = list[list.size() - 1]->GetPos();
		npo += cam->GetForward() * speed* pad.GetLStickYF();
		npo += cam->GetRight() * speed * pad.GetLStickXF();
		list[list.size() - 1]->SetPos(npo);
	}

	auto camtar = cam->GetTarget();
	
	static ue::CVector3 ofs = { 0,0,-200 };
	
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

#if 0
	cam->SetPosition(campos);
	cam->SetTarget(camtar);
	cam->Update();
#endif

	auto v = campos;
	v.Normalize();
	float f = v.Dot(ue::CVector3::AxisZ());
	float deg = ue::CMath::RadToDeg(acos(f));
	v.Cross(ue::CVector3::AxisZ());
	char st[255] = { 0 };
	//sprintf_s(st, "deg:%.3f\n", deg);
	//sprintf_s(st, "x:%.3f y:%.3f z:%.3f\n", v.x,v.y,v.z);
	//OutputDebugString(st);
}
