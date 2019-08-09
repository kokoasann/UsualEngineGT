#pragma once

#include "RenderTarget.h"
#include "LightManager.h"

namespace UsualEngine
{
	/*!
	 *@brief	グラフィックスエンジン。
	 */
	class GraphicsEngine
	{
	public:
		GraphicsEngine();
		~GraphicsEngine();
		/*!
		 *@brief	初期化。
		 *@param[in]	hWnd		ウィンドウハンドル。
		 */
		void Init(HWND hWnd);

		/*!
		 *@brief	解放。
		 */
		void Release();
		/*!
		 *@brief	D3D11デバイスを取得。
		 */
		ID3D11Device* GetD3DDevice()
		{
			return m_pd3dDevice;
		}
		/*!
		 *@brief	D3D11デバイスコンテキストを取得。
		 */
		ID3D11DeviceContext* GetD3DDeviceContext()
		{
			return m_pd3dDeviceContext;
		}

		LightManager& GetLightManager()
		{
			return mLightManager;
		}

		/// <summary>
		/// レンダーターゲットをセットする
		/// </summary>
		/// <param name="targetCount">ターゲットの数</param>
		/// <param name="rtlist">レンダーターゲット配列</param>
		void OMSetRenderTarget(int targetCount, RenderTarget* rtlist[]);

		/// <summary>
		/// 現在のレンダーターゲットをゲットする
		/// </summary>
		/// <param name="targetCount">ターゲットの数が入る</param>
		/// <param name="rtlist">レンダーターゲットが入る</param>
		void OMGetRenderTargets(int& targetCount, RenderTarget* rtlist[]);
		/*!
		 *@brief	描画開始。
		 */
		void BegineRender();
		/*!
		 *@brief	描画終了。
		 */
		void EndRender();
	private:
		D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D デバイスのターゲットとなる機能セット。
		ID3D11Device* m_pd3dDevice = NULL;		//D3D11デバイス。
		IDXGISwapChain* m_pSwapChain = NULL;		//スワップチェイン。
		ID3D11DeviceContext* m_pd3dDeviceContext = NULL;	//D3D11デバイスコンテキスト。
		RenderTarget m_renderTarget;
		ID3D11RenderTargetView* m_backBuffer = NULL;		//バックバッファ。
		ID3D11RasterizerState* m_rasterizerState = NULL;	//ラスタライザステート。
		ID3D11Texture2D* m_depthStencil = NULL;		//デプスステンシル。
		ID3D11DepthStencilView* m_depthStencilView = NULL;	//デプスステンシルビュー。
		

		LightManager mLightManager;						//ライトのマネージャー
		RenderTarget* m_nowRenderTargets[RTV_MAX];		//今のレンダーターゲット
		int m_renderTargetCount = 1;									//今のレンダーターゲットの数
	};

	//extern GraphicsEngine* g_graphicsEngine;			//グラフィックスエンジン
}