#include "PreCompile.h"
#include "system.h"

namespace UsualEngine
{

	///////////////////////////////////////////////////////////////////
	// DirectX�̏I�������B
	///////////////////////////////////////////////////////////////////
	void ReleaseDirectX()
	{

	}
	///////////////////////////////////////////////////////////////////
	//���b�Z�[�W�v���V�[�W���B
	//hWnd�����b�Z�[�W�𑗂��Ă����E�B���h�E�̃n���h���B
	//msg�����b�Z�[�W�̎�ށB
	//wParam��lParam�͈����B���͋C�ɂ��Ȃ��Ă悢�B
	///////////////////////////////////////////////////////////////////
	LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		//�����Ă������b�Z�[�W�ŏ����𕪊򂳂���B
		switch (msg)
		{
		case WM_DESTROY:
			ReleaseDirectX();
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}

}