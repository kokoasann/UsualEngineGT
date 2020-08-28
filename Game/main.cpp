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
	MainCam.SetFar(100000.f);
	MainCam.Update();
	
	ue::NewGO<Game>(0);

	ue->Run();
	return 0;
}