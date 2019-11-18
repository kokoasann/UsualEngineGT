#include "stdafx.h"
#include "Game.h"
#include "Player/Player.h"


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
	animclip[0].Load(L"Assets/model/gib/gib.rotation.tka");
	animclip[0].SetLoopFlag(true);
	animclip[1].Load(L"Assets/model/Player/player_idol.tka");
	animclip[1].SetLoopFlag(true);
	

	ue::CQuaternion rot;
	cam = &ue::usualEngine()->GetMainCamera();
	p1 = ue::NewGO<ue::SkinModelRender>(0);
	p1->Init(L"Assets/model/gib.bone.cmo", animclip, 1,ue::enFbxUpAxisY);
	p1->SetPos({ 0,-60,-1000 });
	p1->SetSca({ 100,100,100 });
	p1->SetIsShadowCaster(true);
	p1->SetIsShadowReciever(true);

	cc.Init(50, 100, { 0,100,-1000 });

	auto& anim = p1->GetAnimation();
	anim.AddEventListener([&](const auto & clipname, const auto & eventname)
	{
		if (eventname.substr(eventname.size() - 2, 2) == "st")
		{
			if (eventname[5] == 'L')
			{
				m_isleftON = true;
				m_isrightON = false;
				count = 0;
			}
			else if (eventname[5] == 'R')
			{
				m_isleftON = false;
				m_isrightON = true;
				count = 0;
			}
		}
	});
	

	for (auto bone : p1->GetSkinModel().GetSkeleton().GetAllBone())
	{
		if (Rfoot==NULL && lstrcmpW(bone->GetName(), L"IK_Bone.007_R.003") == 0)
		{
			Rfoot = bone;
		}
		else if(Lfoot == NULL && lstrcmpW(bone->GetName(), L"IK_Bone.007_L.003") == 0)
		{
			Lfoot = bone;
		}
		else if (Rwaist == NULL && lstrcmpW(bone->GetName(), L"Bone.007_R") == 0)
		{
			Rwaist = bone;
		}
	}
	p1->SetingIK(Rfoot, Rfoot->GetParent()->GetParent()->GetParent(), 70.f);
	p1->SetingIK(Lfoot, Lfoot->GetParent()->GetParent(), 60.f);
	p1->SetingIK(Rfoot->GetChildren()[0], Rfoot, 10.f);
	p1->SetingIK(Lfoot->GetChildren()[0], Lfoot, 10.f);

	p1->SetMoveFunc([&](ue::CVector3 & pos)
	{
		return;
		ue::CVector3 move = ue::CVector3::Zero();
		ue::CVector3 moveR = ue::CVector3::Zero();
		ue::CVector3 moveL = ue::CVector3::Zero();
		if (Rfoot->IsONGround())
		{
			move = Rfoot->GetMove();
		}
		if (Lfoot->IsONGround())
		{
			move += Lfoot->GetMove();
		}
		/*if (moveR.Length() > moveL.Length())
		{
			move = moveR;
		}
		else
		{
			move = moveL;
		}*/
		if (move.Length()>0.0001f)
		{
			move.y = 0;
			move *= -1;
			//move.y *= -1;
			//move.y += -1;
			auto rpos = cc.Execute(1, move);
			rpos.y -= modeloffset;
			pos = rpos;
			//pos += move;
		}
	});
	p1->SetRotateFunc([&](auto & rot)
	{
		//return;
		if (Rfoot->IsONGround())
		{
			auto mold = Rwaist->GetOldWorldMatrix();
			auto mnew = Rwaist->GetBaseWorldMatrix();
			//分改！！
			ue::CQuaternion rold, rnew;
			auto pos = ue::CVector3::Zero(), sca = ue::CVector3::Zero();
			mold.CalcMatrixDecompose(pos, rold, sca);
			mnew.CalcMatrixDecompose(pos, rnew, sca);

			//バインディングポーズの
			ue::CQuaternion rloc;
			auto bmat = Rwaist->GetBindPoseMatrix();
			bmat.CalcMatrixDecompose(pos, rloc, sca);
			ue::CVector3 axis = ue::CVector3::AxisZ();
			rloc.Multiply(axis);
			ue::CVector3 vold = axis, vnew = axis;
			rold.Multiply(vold);
			rnew.Multiply(vnew);

			vold.y = 0;
			vnew.y = 0;

			vold.Normalize();
			vnew.Normalize();

			float rad = vold.Dot(vnew);
			rad = min(rad, 1);
			rad = acos(rad);
			
			ue::CQuaternion add;
			add.SetRotation(ue::CVector3::AxisY(), rad*-1);

			auto fpos = Rfoot->GetWorldMatrix().GetTranslation();
			//fpos -= Rfoot->GetMove();
			auto mpos = p1->GetPos();
			fpos.y = 0;
			mpos.y = 0;
			auto f2m = mpos - fpos;

			add.Multiply(f2m);
			auto npos = fpos + f2m;
			auto m2n = npos - mpos;

			auto rpos = cc.Execute(1, m2n);
			rpos.y -= modeloffset;
			p1->SetPos(rpos);

			auto rot = p1->GetRot();
			rot.Multiply(add);
			p1->SetRot(rot);
		}
#if 0
		{
			ue::CQuaternion adrot;
			adrot.SetRotationDeg(ue::CVector3::AxisY(), 1);

			auto fpos = Rfoot->GetWorldMatrix().GetTranslation();
			//fpos -= Rfoot->GetMove();
			auto mpos = p1->GetPos();
			fpos.y = 0;
			mpos.y = 0;
			auto f2m = mpos - fpos;

			adrot.Multiply(f2m);
			auto npos = fpos + f2m;
			auto m2n = npos - mpos;

			auto rpos = cc.Execute(1, m2n);
			rpos.y -= modeloffset;
			p1->SetPos(rpos);

			auto rot = p1->GetRot();
			rot.Multiply(adrot);
			p1->SetRot(rot);
		}
#endif
	});

	/*p2 = ue::NewGO<ue::SkinModelRender>(0);
	p2->Init(L"Assets/model/Player.cmo" , animclip + 1, 1);
	p2->SetPos({ 0,0,0 });
	p2->SetSca({ 10,10,10 });
	rot.SetRotationDeg(ue::CVector3::AxisX(), 90);
	p2->SetRot(rot);
	p2->SetIsShadowCaster(true);
	p2->SetIsShadowReciever(true);*/

	/*p3 = ue::NewGO<ue::SkinModelRender>(0);
	p3->Init(L"Assets/model/unityChan.cmo");
	p3->SetPos({ 0,-50,1500 });
	p3->SetSca({ 30,30,0.4f });
	
	rot.SetRotationDeg(ue::CVector3::AxisX(), -90);
	p3->SetRot(rot);
	p3->SetIsShadowCaster(true);*/
	


	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/dun.cmo", 0, 0, ue::enFbxUpAxisZ);
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

	auto p = p1->GetPos();

	if (GetAsyncKeyState('Q'))
	{
		p.z += 60;
	}
	else if (GetAsyncKeyState('E'))
	{
		p.z -= 50;
	}
	//ue::CVector3 vv;
	//bool ON = 0;
	//if (m_isrightON)
	//{
	//	vv = Rfoot->GetMove();
	//	ON = 1;
	//}
	//else if(m_isleftON)
	//{
	//	vv = Lfoot->GetMove();
	//	ON = 1;
	//}
	//if (ON)
	//{
	//	vv.y = 0;
	//	p += vv * -1;
	//	
	//	count++;
	//	if (count == 5)
	//	{
	//		//vv= m_movedata[0];
	//		//count = 0;
	//	}
	//	m_movedata[0] = vv;
	//}
	
	 
	
	//ue::CVector3 move;
	//if (Rfoot->IsONGround())
	//{
	//	move = Rfoot->GetMove();
	//}
	//else if (Lfoot->IsONGround())
	//{
	//	move = Lfoot->GetMove();
	//}
	//if (move.y >= 0)
	//{
	//	move.y = 0;
	//	move *= -1;
	//	//move.y += -10;
	//	//auto rpos = cc.Execute(ue::gameTime()->GetDeltaTime(), move);
	//	//rpos.y -= 50;
	//	//pos = rpos;
	//	p += move;
	//}
	
	auto grav = ue::CVector3(0, -50, 0);
	p = cc.Execute(1, grav);
	p.y -= modeloffset;
	p1->SetPos(p);



	ue::CQuaternion add = ue::CQuaternion::Identity();
	
	static std::vector<ue::SkinModelRender*> list;

	if (pad.IsTrigger(ue::enButtonA))
	{
		if (list.size() != 0)
		{
			ue::DeleteGO(list[list.size()-1]);
			list.pop_back();
		}
	}
	else if (pad.IsTrigger(ue::enButtonB))
	{
		ue::SkinModelRender* p1 = ue::NewGO<ue::SkinModelRender>(0);
		p1->Init(L"Assets/model/unityChan.cmo");
		p1->SetPos({ 30,0,50 });
		p1->Update();
		list.push_back(p1);
	}
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

#if 1
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
