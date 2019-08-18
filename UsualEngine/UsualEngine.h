#pragma once
#include "graphics/Camera.h"
namespace UsualEngine
{
	class GraphicsEngine;
	class GameObjectManager;

	struct InitGameState
	{
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
		LPWSTR lpCmdLine;
		int nCmdShow;
		const TCHAR* appName;
	};
	class UsualEngine
	{
	private:
		UsualEngine();
		~UsualEngine();
	public:
		static UsualEngine* Get()
		{
			static UsualEngine* ins = NULL;
			if (ins == NULL)
			{
				ins = new UsualEngine();
			}
			return ins;
		}

		void InitGame(InitGameState initGS);
		void InitWindow(InitGameState initGS);
		bool DispatchWindowMessage();

		void GameLoop();
		void Release();

		void Run();

		HWND GetHwnd()
		{
			return mHwnd;
		}
		GraphicsEngine* GetGraphicsEngine()
		{
			return mGraphicsEngine;
		}
		Camera& GetMainCamera()
		{
			return mMainCamera;
		}
		Camera& Get2DCamera()
		{
			return mCamera2D;
		}
	private:
		HWND mHwnd = 0;
		GraphicsEngine* mGraphicsEngine = nullptr;
		GameObjectManager* mGameObjectManager = nullptr;
		Camera mMainCamera;
		Camera mCamera2D;

		int m_count = 0;
		float m_sumTime = 0.f;
	};

	static UsualEngine* usualEngine()
	{
		return UsualEngine::Get();
	}
}