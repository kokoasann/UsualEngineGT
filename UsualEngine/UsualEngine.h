#pragma once
#include "graphics/Camera.h"
#include "physics/Physics.h"
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
			return m_hwnd;
		}
		GraphicsEngine* GetGraphicsEngine()
		{
			return m_graphicsEngine;
		}
		Camera& GetMainCamera()
		{
			return m_mainCamera;
		}
		Camera& Get2DCamera()
		{
			return m_camera2D;
		}
		Pad& GetPad(int no)
		{
			return m_pad[no];
		}

		PhysicsWorld& GetPhysics()
		{
			return m_physics;
		}
	private:
		HWND m_hwnd = 0;
		GraphicsEngine* m_graphicsEngine = nullptr;
		GameObjectManager* m_gameObjectManager = nullptr;
		Camera m_mainCamera;
		Camera m_camera2D;
		Pad m_pad[Pad::CONNECT_PAD_MAX];
		PhysicsWorld m_physics;

		int m_count = 0;
		float m_sumTime = 0.f;

#if DEBUG_FUNC
		Font m_font;
		float m_fps = 0.f;
		static const bool DRAW_FPS = false;
#endif
	};

	static UsualEngine* usualEngine()
	{
		return UsualEngine::Get();
	}
	static inline Pad& GamePad(int i)
	{
		return usualEngine()->GetPad(i);
	}
	static PhysicsWorld& Physics()
	{
		return usualEngine()->GetPhysics();
	}
}