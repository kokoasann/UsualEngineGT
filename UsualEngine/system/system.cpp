#include "PreCompile.h"
#include "system.h"

namespace UsualEngine
{

	///////////////////////////////////////////////////////////////////
	// DirectXの終了処理。
	///////////////////////////////////////////////////////////////////
	void ReleaseDirectX()
	{

	}
	///////////////////////////////////////////////////////////////////
	//メッセージプロシージャ。
	//hWndがメッセージを送ってきたウィンドウのハンドル。
	//msgがメッセージの種類。
	//wParamとlParamは引数。今は気にしなくてよい。
	///////////////////////////////////////////////////////////////////
	LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		//送られてきたメッセージで処理を分岐させる。
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