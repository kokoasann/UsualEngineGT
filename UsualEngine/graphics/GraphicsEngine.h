#pragma once

#include "RenderTarget.h"
#include "LightManager.h"
#include "ShadowMap.h"
#include "PostEffect/PostEffect.h"
#include "PreRender.h"

namespace UsualEngine
{
	/// <summary>
	/// 
	/// </summary>
	enum EnRenderMode
	{
		enRenderMode_ShadowMap,		//シャドウマップへ描画
		enRenderMode_3DModel,			//3Dモデルを描画
	};

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
		void InitBackBuffer();
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
			return m_lightManager;
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

		void EndPostEffect();

		void PreRenderDraw();

		ShadowMap& GetShadowMap()
		{
			return m_shadowMap;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rm"></param>
		void SetRenderMode(EnRenderMode rm)
		{
			m_renderMode = rm;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		EnRenderMode GetRenderMode()
		{
			return m_renderMode;
		}

		/// <summary>
		/// メインレンダーターゲットを返す
		/// </summary>
		/// <returns></returns>
		RenderTarget* GetMainRenderTarget()
		{
			return &m_mainRenderTarget;
		}

		/// <summary>
		/// ポストエッフェクトのインッスタンス。
		/// </summary>
		/// <returns></returns>
		PostEffect& GetPostEffect()
		{
			return m_postEffect;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		PreRender& GetPreRender()
		{
			return m_preRender;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		DirectX::SpriteBatch* GetSpriteBatch()
		{
			return m_spriteBatch;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		DirectX::SpriteFont* GetSpriteFont()
		{
			return m_spriteFont;
		}
	private:
		D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D デバイスのターゲットとなる機能セット。
		ID3D11Device* m_pd3dDevice = NULL;					//D3D11デバイス。
		IDXGISwapChain* m_pSwapChain = NULL;				//スワップチェイン。
		ID3D11DeviceContext* m_pd3dDeviceContext = NULL;	//D3D11デバイスコンテキスト。
		RenderTarget m_mainRenderTarget;
		ID3D11RenderTargetView* m_backBuffer = NULL;		//バックバッファ。
		ID3D11RasterizerState* m_rasterizerState = NULL;	//ラスタライザステート。
		ID3D11Texture2D* m_depthStencil = NULL;				//デプスステンシル。
		ID3D11DepthStencilView* m_depthStencilView = NULL;	//デプスステンシルビュー。

		Shader m_vsCopy;									//コピー用頂点シェーダ
		Shader m_psCopy;									//コピー用ピクセルシェーダ
		Shader m_vsDefferd;								//デファード用頂点シェーダ
		Shader m_psDefferd;								//デファード用ピクセルシェーダ
#if DEBUG_FUNC
		Shader m_psDefferd_Diffuse;						//デファード用ピクセルシェーダ(デバック用)
		Shader m_psDefferd_Normal;						//デファード用ピクセルシェーダ(デバック用)
		Shader m_psDefferd_Depth;						//デファード用ピクセルシェーダ(デバック用)
		Shader m_psDefferd_Shadow;						//デファード用ピクセルシェーダ(デバック用)
		Shader m_psDefferd_Specular;					//デファード用ピクセルシェーダ(デバック用)
#endif
		
		ShadowMap m_shadowMap;								//シャドウマップ
		LightManager m_lightManager;							//ライトのマネージャー
		RenderTarget* m_nowRenderTargets[RTV_MAX] = { 0 };	//今のレンダーターゲット
		int m_renderTargetCount = 1;						//今のレンダーターゲットの数

		PreRender m_preRender;							//プレレンダー
		PostEffect m_postEffect;							//ポストエッフェクト
		
		EnRenderMode m_renderMode = enRenderMode_3DModel;	//今どの工程？

		ID3D11ShaderResourceView* m_speculaGradation=nullptr;		//トゥーンシェード用のグラデーションマップ
		DirectX::SpriteBatch* m_spriteBatch = nullptr;							//スプライトバッチ
		DirectX::SpriteFont* m_spriteFont = nullptr;								//スプライトフォント
	};
}