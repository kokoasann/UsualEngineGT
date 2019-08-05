#include "stdafx.h"
#include "Game.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UsualEngine::InitGameState IGS;
	IGS.appName = "GAME";
	IGS.hInstance = hInstance;
	IGS.hPrevInstance = hPrevInstance;
	IGS.lpCmdLine = lpCmdLine;
	IGS.nCmdShow = nCmdShow;
	UsualEngine::UsualEngine* ue = UsualEngine::usualEngine();
	ue->InitGame(IGS);

	UsualEngine::Camera& MainCam = UsualEngine::usualEngine()->GetMainCamera();
	MainCam.SetPosition({ 0,80,120 });
	MainCam.SetTarget({ 0,80,0 });
	MainCam.Update();
	
	ue::NewGO<Game>(0);
	//UsualEngine::SkinModelRender* smr = UsualEngine::NewGO<UsualEngine::SkinModelRender>(0,"");
	//smr->Init(L"Assets/model/unityChan.cmo");

	//UsualEngine::SpriteRender* sr = UsualEngine::NewGO<UsualEngine::SpriteRender>(0);
	//sr->Init(L"Assets/sprite/daru.dds", {1280,720});
	//sr->SetPos({ 0,0,500 });
	//sr->SetSca(ue::CVector3::One() * 10);

	ue::LightDirection* light = ue::NewGO<ue::LightDirection>(0);
	/*light = ue::NewGO<ue::LightDirection>(0);
	light->SetDir({ 0,-1,-0.707f });
	light->SetCol(ue::CVector4{ 0.7f,0.2f,0.1f,1.f }/2.f);
	light = ue::NewGO<ue::LightDirection>(0);
	light->SetDir({ 0.3f,-1,-0.707f });
	light->SetCol(ue::CVector4{ 0.2f,0.2f,0.8f,1.f } / 2.f);*/
	ue->Run();
	return 0;
}