#include "PreCompile.h"
#include "UsualEngine.h"

#include "GameObject/GameObjectManager.h"

#include "system/system.h"
#if DEBUG_FUNC
#include "Debug/DebugSwitch.h"
#endif
#include "Debug/Debug.h"

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
		m_graphicsEngine = new GraphicsEngine();
		m_graphicsEngine->Init(m_hwnd);

		m_gameObjectManager = GameObjectManager::Get();

		//ゲームパッドの初期化。
		//最大４つのコントローラーを接続できるようにしましょう。
		m_pad[0].Init(0);
		m_pad[1].Init(1);
		m_pad[2].Init(2);
		m_pad[3].Init(3);

		//2Dカメラの初期化
		m_camera2D.SetPosition({ 0,0,-500 });
		m_camera2D.CameraModeOrth();
		m_camera2D.Update();

		Physics().Init();
		Debug::Instance().InitDebugSwitch();
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
		m_hwnd = CreateWindow(
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

		ShowWindow(m_hwnd, initGS.nCmdShow);
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
			for (auto& pad : m_pad)
			{
				pad.Update();
			}

			DebugSwitchUpdate();

			m_graphicsEngine->BegineRender();
			m_graphicsEngine->GetShadowMap().Update();
			m_graphicsEngine->SetRenderMode(enRenderMode_ShadowMap);
			m_graphicsEngine->GetShadowMap().Render();
			m_graphicsEngine->SetRenderMode(enRenderMode_3DModel);
			LightManager& lm = m_graphicsEngine->GetLightManager();
			lm.Update();
			lm.Render();

			m_graphicsEngine->GetShadowMap().Send2GPU();

			m_gameObjectManager->Update();
			m_physics.Update();
			lm.EndRender();

			m_graphicsEngine->EndRender();

			float frameTime = st.Stop();
			gameTime()->PushFrameDeltaTime(frameTime);

			m_count++;
			if (m_count >= 30)
			{
				char s[25] = { 0 };
				sprintf_s(s, "%f3.2\n", 1.f / (m_sumTime / 30.f));
				OutputDebugStringA(s);
				m_sumTime = 0.f;
				m_count = 0;
			}
			m_sumTime += frameTime;

		}
	}
	void UsualEngine::Release()
	{
		m_graphicsEngine->Release();
	}
	void UsualEngine::Run()
	{
		GameLoop();
		Release();
	}
}