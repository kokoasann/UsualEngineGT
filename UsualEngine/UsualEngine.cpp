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
		//�E�B���h�E���������B
		InitWindow(initGS);
		//DirectX�̏������B
		m_graphicsEngine = new GraphicsEngine();
		m_graphicsEngine->Init(m_hwnd);

		m_gameObjectManager = GameObjectManager::Get();

		//�Q�[���p�b�h�̏������B
		//�ő�S�̃R���g���[���[��ڑ��ł���悤�ɂ��܂��傤�B
		m_pad[0].Init(0);
		m_pad[1].Init(1);
		m_pad[2].Init(2);
		m_pad[3].Init(3);

		//2D�J�����̏�����
		m_camera2D.SetPosition({ 0,0,-500 });
		m_camera2D.CameraModeOrth();
		m_camera2D.Update();

		Physics().Init();
		Debug::Instance().InitDebugSwitch();
	}

	void UsualEngine::InitWindow(InitGameState initGS)
	{
		//�E�B���h�E�N���X�̃p�����[�^��ݒ�(�P�Ȃ�\���̂̕ϐ��̏������ł��B)
		WNDCLASSEX wc =
		{
			sizeof(WNDCLASSEX),		//�\���̂̃T�C�Y�B
			CS_CLASSDC,				//�E�B���h�E�̃X�^�C���B
									//�����̎w��ŃX�N���[���o�[��������ł��邪�A�Q�[���ł͕s�v�Ȃ̂�CS_CLASSDC�ł悢�B
			MsgProc,				//���b�Z�[�W�v���V�[�W��(��q)
			0,						//0�ł����B
			0,						//0�ł����B
			GetModuleHandle(NULL),	//���̃N���X�̂��߂̃E�C���h�E�v���V�[�W��������C���X�^���X�n���h���B
									//�����C�ɂ��Ȃ��Ă悢�B
			NULL,					//�A�C�R���̃n���h���B�A�C�R����ς������ꍇ������ύX����B�Ƃ肠��������ł����B
			NULL,					//�}�E�X�J�[�\���̃n���h���BNULL�̏ꍇ�̓f�t�H���g�B
			NULL,					//�E�B���h�E�̔w�i�F�BNULL�̏ꍇ�̓f�t�H���g�B
			NULL,					//���j���[���BNULL�ł����B
			initGS.appName,				//�E�B���h�E�N���X�ɕt���閼�O�B
			NULL					//NULL�ł����B
		};
		//�E�B���h�E�N���X�̓o�^�B
		RegisterClassEx(&wc);

		// �E�B���h�E�̍쐬�B
		m_hwnd = CreateWindow(
			initGS.appName,		//�g�p����E�B���h�E�N���X�̖��O�B
									//��قǍ쐬�����E�B���h�E�N���X�Ɠ������O�ɂ���B
			initGS.appName,		//�E�B���h�E�̖��O�B�E�B���h�E�N���X�̖��O�ƕʖ��ł��悢�B
			WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C���B�Q�[���ł͊�{�I��WS_OVERLAPPEDWINDOW�ł����A
			0,						//�E�B���h�E�̏���X���W�B
			0,						//�E�B���h�E�̏���Y���W�B
			(UINT)FRAME_BUFFER_W,	//�E�B���h�E�̕��B
			(UINT)FRAME_BUFFER_H,	//�E�B���h�E�̍����B
			NULL,					//�e�E�B���h�E�B�Q�[���ł͊�{�I��NULL�ł����B
			NULL,					//���j���[�B����NULL�ł����B
			initGS.hInstance,				//�A�v���P�[�V�����̃C���X�^���X�B
			NULL
		);

		ShowWindow(m_hwnd, initGS.nCmdShow);
	}

	bool UsualEngine::DispatchWindowMessage()
	{
		MSG msg = { 0 };
		while (WM_QUIT != msg.message) {
			//�E�B���h�E����̃��b�Z�[�W���󂯎��B
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				//�E�B���h�E���b�Z�[�W����ɂȂ����B
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