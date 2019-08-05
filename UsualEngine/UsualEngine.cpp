#include "PreCompile.h"
#include "UsualEngine.h"

#include "GameObject/GameObjectManager.h"

#include "system/system.h"

namespace UsualEngine
{
	UsualEngine::UsualEngine()
	{
		
	}


	UsualEngine::~UsualEngine()
	{
	}

	void UsualEngine::InitGame(InitGameState initGS)
	{
		//ウィンドウを初期化。
		InitWindow(initGS);
		//DirectXの初期化。
		mGraphicsEngine = new GraphicsEngine();
		mGraphicsEngine->Init(mHwnd);

		mGameObjectManager = GameObjectManager::Get();

		//ゲームパッドの初期化。
		//最大４つのコントローラーを接続できるようにしましょう。
		g_pad[0].Init(0);
		g_pad[1].Init(1);
		g_pad[2].Init(2);
		g_pad[3].Init(3);

		//2Dカメラの初期化
		mCamera2D.SetPosition({ 0,0,-500 });
		mCamera2D.CameraModeOrth();
		mCamera2D.Update();

		g_physics.Init();
	}

	void UsualEngine::InitWindow(InitGameState initGS)
	{
		//ウィンドウクラスのパラメータを設定(単なる構造体の変数の初期化です。)
		WNDCLASSEX wc =
		{
			sizeof(WNDCLASSEX),		//構造体のサイズ。
			CS_CLASSDC,				//ウィンドウのスタイル。
									//ここの指定でスクロールバーをつけたりできるが、ゲームでは不要なのでCS_CLASSDCでよい。
			MsgProc,				//メッセージプロシージャ(後述)
			0,						//0でいい。
			0,						//0でいい。
			GetModuleHandle(NULL),	//このクラスのためのウインドウプロシージャがあるインスタンスハンドル。
									//何も気にしなくてよい。
			NULL,					//アイコンのハンドル。アイコンを変えたい場合ここを変更する。とりあえずこれでいい。
			NULL,					//マウスカーソルのハンドル。NULLの場合はデフォルト。
			NULL,					//ウィンドウの背景色。NULLの場合はデフォルト。
			NULL,					//メニュー名。NULLでいい。
			initGS.appName,				//ウィンドウクラスに付ける名前。
			NULL					//NULLでいい。
		};
		//ウィンドウクラスの登録。
		RegisterClassEx(&wc);

		// ウィンドウの作成。
		mHwnd = CreateWindow(
			initGS.appName,		//使用するウィンドウクラスの名前。
									//先ほど作成したウィンドウクラスと同じ名前にする。
			initGS.appName,		//ウィンドウの名前。ウィンドウクラスの名前と別名でもよい。
			WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル。ゲームでは基本的にWS_OVERLAPPEDWINDOWでいい、
			0,						//ウィンドウの初期X座標。
			0,						//ウィンドウの初期Y座標。
			(UINT)FRAME_BUFFER_W,	//ウィンドウの幅。
			(UINT)FRAME_BUFFER_H,	//ウィンドウの高さ。
			NULL,					//親ウィンドウ。ゲームでは基本的にNULLでいい。
			NULL,					//メニュー。今はNULLでいい。
			initGS.hInstance,				//アプリケーションのインスタンス。
			NULL
		);

		ShowWindow(mHwnd, initGS.nCmdShow);
	}

	bool UsualEngine::DispatchWindowMessage()
	{
		MSG msg = { 0 };
		while (WM_QUIT != msg.message) {
			//ウィンドウからのメッセージを受け取る。
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				//ウィンドウメッセージが空になった。
				break;
			}
		}
		return msg.message != WM_QUIT;
	}

	void UsualEngine::GameLoop()
	{
		while (DispatchWindowMessage() == true)
		{
			Stopwatch st;
			st.Start();

			mGraphicsEngine->BegineRender();

			LightManager& lm = mGraphicsEngine->GetLightManager();
			lm.Update();
			lm.Render();

			mGameObjectManager->Update();

			lm.EndRender();

			mGraphicsEngine->EndRender();

			float frameTime = st.Stop();
			gameTime()->PushFrameDeltaTime(frameTime);
		}
	}
	void UsualEngine::Release()
	{
		mGraphicsEngine->Release();
	}
	void UsualEngine::Run()
	{
		GameLoop();
		Release();
	}
}