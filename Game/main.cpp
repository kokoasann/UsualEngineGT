#include "stdafx.h"

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
	MainCam.SetPosition({ 0,0,500 });
	MainCam.Update();
	

	//UsualEngine::SkinModelRender* smr = UsualEngine::NewGO<UsualEngine::SkinModelRender>(0,"");
	//smr->Init(L"Assets/model/unityChan.cmo");

	UsualEngine::SpriteRender* sr = UsualEngine::NewGO<UsualEngine::SpriteRender>(0);
	sr->Init(L"Assets/sprite/daru.dds", {10,10});
	sr->SetPos({ 0,0,500 });
	//sr->SetSca(ue::CVector3::One() * 10);
	
	ue->Run();
	return 0;
}