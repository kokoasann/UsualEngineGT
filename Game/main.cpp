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
	

	UsualEngine::SkinModelRender* smr = UsualEngine::NewGO<UsualEngine::SkinModelRender>(0,"");
	smr->Init(L"Assets/model/unityChan.cmo");

	ue->Run();
	return 0;
}