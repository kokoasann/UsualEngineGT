#include "stdafx.h"
#include "Game.h"
#include "Player/Player.h"
#include "Enemy/Gib/Ene_Gib.h"
#include "_Test/Ene_Tes.h"
#include "Physics/BoxCollider.h"

Game::Game()
{
	m_fblur.Init();
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

	//インスタンシングのテスト
	if(0)
	{
		ue::CVector3 sca = ue::CVector3(0.05, 0.05, 0.05);
		ue::CMatrix mat;
		mat.MakeScaling(sca);
		
		ue::SkinModelRender* skinmodel = ue::NewGO<ue::SkinModelRender>(0);
		skinmodel->Init(L"Assets/model/dun.cmo", 0, 0);
		auto& ssss = skinmodel->GetSkinModel();
		ssss.InitInstancing(2);
		mat.v[3] = { 10,50,0,1 };
		ssss.SetInstanceData(0,mat);
		mat.v[3] = { 100,50,0,1 };
		ssss.SetInstanceData(1, mat);
	}

	//アルファモデルのテスト
	{
		ue::SkinModelRender* alpha = ue::NewGO<ue::SkinModelRender>(0);
		alpha->Init(L"Assets/model/AlphaModelTestBlock.cmo");
		alpha->SetAlphaMap(L"Assets/sprite/kusa_spe.dds", L"Alpha");
		//alpha->SetSca({ 10,10,10 });
		alpha->SetPos({ 200,0,-100 });
		alpha->SetRefractiveIndex(1.0f);
		
		alpha = ue::NewGO<ue::SkinModelRender>(0);
		alpha->Init(L"Assets/model/AlphaModelTestBlock.cmo");
		alpha->SetAlphaMap(L"Assets/sprite/kusa_spe.dds", L"Alpha");
		//alpha->SetSca({ 10,10,10 });
		alpha->SetPos({ 500,0,-100 });
		alpha->SetRefractiveIndex(1.5f);

		alpha = ue::NewGO<ue::SkinModelRender>(0);
		alpha->Init(L"Assets/model/AlphaModelTestCapsule.cmo");
		alpha->SetAlphaMap(L"Assets/sprite/kusa_spe.dds", L"Alpha");
		//alpha->SetSca({ 10,10,10 });
		alpha->SetPos({ 800,0,-100 });
		alpha->SetRefractiveIndex(1.5f);

		alpha = ue::NewGO<ue::SkinModelRender>(0);
		alpha->Init(L"Assets/model/AlphaModelTest.cmo");
		alpha->SetAlphaMap(L"Assets/sprite/kusa_spe.dds", L"Alpha");
		//alpha->SetSca({ 10,10,10 });
		alpha->SetPos({ 1100,0,-100 });
		alpha->SetRefractiveIndex(1.5f);
	}

	
	ue::SkinModelRender* weedSM = ue::NewGO<ue::SkinModelRender>(0);
	weedSM->Init(L"Assets/model/weed.cmo", 0, 0, ue::enFbxUpAxisY);
	weedSM->SetIsShadowCaster(true);
	weedSM->SetIsShadowReciever(true);
	auto& weedSMs = weedSM->GetSkinModel();
	weedSMs.InitInstancing(3864);

	int grCount = 0;
	int weedinst = 0;
	ue::Level level;
	level.Init(L"Assets/level/level_test.tkl", [&](ue::LevelObjectData& obj)->bool
	{
			if (wcscmp(obj.name, L"map1A")==0 ||
				wcscmp(obj.name, L"map2A") == 0 ||
				wcscmp(obj.name, L"map3A") == 0 || 
				wcscmp(obj.name, L"map1B") == 0 ||
				wcscmp(obj.name, L"map2B") == 0 ||
				wcscmp(obj.name, L"map3B") == 0 ||
				wcscmp(obj.name, L"map1C") == 0 ||
				wcscmp(obj.name, L"map2C") == 0 ||
				wcscmp(obj.name, L"map3C") == 0)
			{
				float sca = 0.04;
				ue::CVector3 pos = { 0,-7600 * sca,0 };
				ue::CVector3 scal = ue::CVector3::One() * sca;
				std::wstring path = L"Assets/model/";
				path += obj.name;
				path += L".cmo";
				ground = ue::NewGO<ue::SMR4Ground>(0);
				ground->InitG(path.c_str(), 0, 0, ue::enFbxUpAxisZ, ue::SMR4Ground::gtBlendThree);
				ground->SetIsShadowCaster(true);
				ground->SetIsShadowReciever(true);
				ground->SetBlendMap(L"Assets/sprite/worldBlend.dds");
				ground->SetTexture(0, L"Assets/sprite/kusa.dds");
				ground->SetTexture(1, L"Assets/sprite/tuti.dds");
				ground->SetTexture(2, L"Assets/sprite/iwa.dds");
				ground->SetSpecularMap(ue::SMR4Ground::tkGrass, L"Assets/sprite/kusa_spe.dds", L"grass");
				ground->SetSpecularMap(ue::SMR4Ground::tkTuti, L"Assets/sprite/tuti_spe.dds", L"gaia");
				ground->SetSpecularMap(ue::SMR4Ground::tkOther, L"Assets/sprite/iwa_spe.dds", L"");
				ground->SetPos(pos);
				ground->SetSca(scal);
				auto* colli = ue::NewGO<ue::SkinModelRender>(0);
				colli->SetActive(false);
				path = L"Assets/model/";
				path += obj.name;
				path += L"_col.cmo";
				colli->Init(path.c_str());
				wpso[grCount].CreateMeshObject(colli->GetSkinModel(), ground->GetPos(), colli->GetRot(), ground->GetSca());
				auto& collider = wpso[grCount].GetMeshCollider();
				auto b = collider.GetBody();
				
				auto& rig = wpso[grCount].GetRigidBody();
				//auto b = rig.GetBody();
				
				grCount++;
			}
			else if(wcscmp(obj.name,L"weed")==0)
			{
				ue::CMatrix mat;
				mat.MakeScaling(obj.scale*0.4f);
				mat.v[3] = ue::CVector4(obj.position)*120;
				mat.v[3].w = 1;
				weedSMs.SetInstanceData(weedinst, mat);
				weedinst++;
			}
			return true;
	});

	auto BG = ue::NewGO < ue::SkinModelRender>(0);
	BG->Init(L"Assets/model/BackGroundTest.cmo");
	BG->SetSca(ue::CVector3::One() * 50);

	cam = &ue::usualEngine()->GetMainCamera();

	//IKのデバッグ用の平らな足場とスロープ
#if 0
	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/dun.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetIsShadowCaster(true);
	ground->SetIsShadowReciever(true);
	//ground->SetSca(ue::CVector3{30, 0.1f, 10});
	ground->SetSca(ue::CVector3{30, 30.f, 10}/10);
	//ground->SetPos({ 0,-100,0 });
	ground->SetPos(ue::CVector3{ 0,-100*300,0 }/10);
	rot.SetRotationDeg(ue::CVector3::AxisZ(), 90);
	ground->SetBlendMap(L"Assets/sprite/map.dds");
	ground->SetTexture(0, L"Assets/sprite/kusa.dds");
	ground->SetTexture(1, L"Assets/sprite/tuti.dds");
	ground->SetSpecularMap(ue::SMR4Ground::tkGrass, L"Assets/sprite/kusa_spe.dds", L"grass");
	ground->SetSpecularMap(ue::SMR4Ground::tkTuti, L"Assets/sprite/tuti_spe.dds", L"gaia");

	pso.CreateMeshObject(ground->GetSkinModel(), ground->GetPos(), ground->GetRot(),ground->GetSca());
	ground->SetRot(rot);
	//デバッグ用のスロープ
	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/dun.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetIsShadowCaster(true);
	ground->SetIsShadowReciever(true);
	ground->SetSca(ue::CVector3{ 30, 1, 50 }/10);
	ground->SetPos(ue::CVector3{ 0,-900,400 }/10);
	ue::CQuaternion rot;
	rot.SetRotationDeg(ue::CVector3::AxisX(), -20);
	ground->SetRot(rot);
	ground->SetBlendMap(L"Assets/sprite/map.dds");
	ground->SetTexture(0, L"Assets/sprite/kusa.dds");
	ground->SetTexture(1, L"Assets/sprite/tuti.dds");
	ground->SetSpecularMap(ue::SMR4Ground::tkGrass, L"Assets/sprite/kusa_spe.dds", L"grass");
	ground->SetSpecularMap(ue::SMR4Ground::tkTuti, L"Assets/sprite/tuti_spe.dds", L"gaia");
	pso2.CreateMeshObject(ground->GetSkinModel(), ground->GetPos(), ground->GetRot(), ground->GetSca());
#endif

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/octagon.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{ 7, 9.2f, 7 }/10);
	ground->SetPos(ue::CVector3{0,0,-20000 }/10);
	ground->SetIsShadowCaster(true);
	ground->SetIsShadowReciever(true);
	ground->SetSpecularMap(ue::SMR4Ground::tkGrass, L"Assets/sprite/kusa_spe.dds",L"grass");
	ground->SetSpecularMap(ue::SMR4Ground::tkTuti, L"Assets/sprite/tuti_spe.dds",L"gaia");

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/octagon.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{ 1.5, 2.2f, 1.5 }/10);
	ground->SetPos(ue::CVector3{ -1500,0,-10000 }/10);
	ground->SetIsShadowCaster(true);
	ground->SetIsShadowReciever(true);
	ground->SetSpecularMap(ue::SMR4Ground::tkGrass, L"Assets/sprite/kusa_spe.dds", L"grass");
	ground->SetSpecularMap(ue::SMR4Ground::tkTuti, L"Assets/sprite/tuti_spe.dds", L"gaia");

	ground = ue::NewGO<ue::SMR4Ground>(0);
	ground->InitG(L"Assets/model/octagon.cmo", 0, 0, ue::enFbxUpAxisZ);
	ground->SetSca(ue::CVector3{ 0.4, 1.2f, 0.4 }/10);
	ground->SetPos(ue::CVector3{ 700,0,-3000 }/10);
	ground->SetIsShadowCaster(true);
	ground->SetIsShadowReciever(true);
	ground->SetSpecularMap(ue::SMR4Ground::tkGrass, L"Assets/sprite/kusa_spe.dds", L"grass");
	ground->SetSpecularMap(ue::SMR4Ground::tkTuti, L"Assets/sprite/tuti_spe.dds", L"gaia");

	light = ue::NewGO<ue::LightDirection>(0);
	light->SetDir(ue::CVector3{ -0.3f,-1,-0.2f });
	light->SetCol(ue::CVector3::One()/ 3.5f);
	/*light = ue::NewGO<ue::LightDirection>(0);
	light->SetDir(ue::CVector3(1.f, 0.0f, 0.f));
	light->SetCol({ 0.3f,0.0f,0.f,1.0f });*/
	

	static auto plig = ue::NewGO<ue::LightPoint>(0);
	plig->SetPos({ 0,1,-100 });
	auto col = ue::CVector3{ 0.6,0.25,0.08 };
	col.Normalize();
	plig->SetCol(col*5);
	plig->SetDecay(0.001f);
	plig->SetRadius(300.f);

	campos = { -240,52,-100 };
	//campos = { 0,150,180 };
	cam->SetPosition(campos);
	cam->SetTarget({ 0,2,-100 });
	//cam->SetTarget({ 0,50,50 });
	cam->Update();
	
	std::function<void()> f = [&]()
	{
		auto cam = &ue::usualEngine()->GetMainCamera();
		auto& pad = ue::GamePad(0);
		//return;

		ue::CQuaternion add = ue::CQuaternion::Identity();

		auto camtar = cam->GetTarget();

		static ue::CVector3 ofs = { 0,0,-200 };

		float dtime = ue::gameTime()->GetDeltaTime();
		float speed = 30.f * dtime;
		campos += cam->GetForward() * speed * pad.GetLStickYF();
		campos += cam->GetRight() * speed * pad.GetLStickXF();


		float yx = pad.GetRStickXF();
		float yy = pad.GetRStickYF();
		speed = 180.f * dtime;
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
	};
	ue::DebugSwitchAddRadio(ue::DebugSwitchNewSwitch('Z', VK_SHIFT, f));

	f = [&]()
	{
		static bool isUp = true;
		if (plig->GetPos().y >= 200 && isUp)
		{
			isUp = false;
			ue::DebugPrint(L"ポイントライトDOWN！");
		}
		else if(plig->GetPos().y <= 1 && !isUp)
		{
			isUp = true;
			ue::DebugPrint(L"ポイントライトUP！");
		}
		auto pos = plig->GetPos();
		if (isUp)
		{
			pos.y += 80.0f * ue::gameTime()->GetDeltaTime();
			plig->SetPos(pos);
		}
		else
		{
			pos.y -= 80.0f * ue::gameTime()->GetDeltaTime();
			plig->SetPos(pos);
		}
	};
	ue::DebugSwitchAddCheck(ue::DebugSwitchNewSwitch('P', 0, f));


	// font blur の　設定。
	m_fblur.SetSpeed(0.5f);
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

	//
	/*static float blurParam = FLT_EPSILON;
	static bool  isUP = true;
	static float upSpeed = 50.f;
	static float sspeed[3] = { upSpeed,upSpeed / 1.f,upSpeed / 1.f };
	static float pmax = 64;
	if (isUP)
	{
		if(blurParam <16.f)
			blurParam += sspeed[0] *ue::gameTime()->GetDeltaTime();
		else if(blurParam < 32.f)
			blurParam += sspeed[1] * ue::gameTime()->GetDeltaTime();
		else
			blurParam += sspeed[2] * ue::gameTime()->GetDeltaTime();
		if (blurParam > pmax)
		{
			blurParam = pmax;
			isUP = false;
		}
	}
	else
	{
		if (blurParam >= 32.f)
			blurParam -= sspeed[2] * ue::gameTime()->GetDeltaTime();
		else if (blurParam >= 16.f)
			blurParam -= sspeed[1] * ue::gameTime()->GetDeltaTime();
		else
			blurParam -= sspeed[0] * ue::gameTime()->GetDeltaTime();
		
		if (blurParam <= FLT_EPSILON)
		{
			blurParam = FLT_EPSILON;
			isUP = true;
		}
	}
	m_fblur.SetBlurParam(blurParam);*/
	
}

void Game::PostRender()
{
#if 0
	m_fblur.Update();
	
	static bool isUped = false;
	if (m_fblur.IsStop())
	{
		if (!isUped)
		{
			m_fblur.Up();
			//m_fblur.Reset();
			isUped = true;
		}
		else
		{
			m_fblur.Down();
			isUped = false;
		}
	}
	auto str = L"O";
	m_font.Begin();

	//m_font.Draw(str, { 0,0 }, { 1,1,1,1 }, 0, 5, { 0.5f,-1.f });
	

	m_font.End();

	m_fblur.DrawStart({1,1,1,0});
	m_font.Begin();

	m_font.Draw(str, { 0,0 }, { 1,1,1,1 }, 0, 5,{0.5f,-1.f});
	//m_font.Draw(L"YESY", {})

	m_font.End();
	m_fblur.DrawEnd();
#endif

}
