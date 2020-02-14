#pragma once

#include "Wraped/ViewPort.h"
namespace UsualEngine
{
	/// <summary>
	/// 
	/// </summary>
	class RenderTarget
	{
	public:
		RenderTarget();
		~RenderTarget();

		void Release();

		/// <summary>
		/// レンダリングターゲットの作成
		/// </summary>
		/// <param name="w">幅</param>
		/// <param name="h">高さ</param>
		/// <param name="mipLevel">ミップマップレベル</param>
		/// <param name="arraySize">配列のサイズ</param>
		/// <param name="colformat">カラーバッファのフォーマット</param>
		/// <param name="dsFormat">深度ステンシルのフォーマット</param>
		/// <param name="msDesc">マルチサンプリングの定義</param>
		/// <param name="renderTarget">レンダーターゲットのテクスチャ。NULLなら勝手に作る</param>
		/// <param name="depthStencil">デプスステンシルのテクスチャ。NULLなら勝手に作る</param>
		/// <returns>成功したらtrueを返す</returns>
		bool Create(int w, int h, int mipLevel, int arraySize, DXGI_FORMAT colformat, DXGI_FORMAT dsFormat,
			DXGI_SAMPLE_DESC msDesc, ID3D11Texture2D* renderTarget = nullptr, ID3D11Texture2D* depthStencil = nullptr, bool RTVDisNone =false);

		/// <summary>
		/// renderTargetViewを返す
		/// </summary>
		/// <returns></returns>
		ID3D11RenderTargetView* GetRTV()
		{
			return m_renderTargetView;
		}

		/// <summary>
		/// depthStencilViewを返す
		/// </summary>
		/// <returns></returns>
		ID3D11DepthStencilView* GetDSV() const
		{
			return m_depthStencilView;
		}

		/// <summary>
		/// shaderResourceViewを返す
		/// </summary>
		/// <returns></returns>
		ID3D11ShaderResourceView*& GetSRV()
		{
			return m_renderTargetSRV;
		}

		/// <summary>
		/// レンダーターゲットを返す。
		/// </summary>
		/// <returns></returns>
		ID3D11Texture2D* GetRenderTarget() const
		{
			return m_renderTarget;
		}

		/// <summary>
		/// テクスチャのフォーマットを返す
		/// </summary>
		/// <returns></returns>
		const DXGI_FORMAT* GetTexFormat() const
		{
			return &m_colFormat;
		}

		/// <summary>
		/// 横を返す
		/// </summary>
		/// <returns></returns>
		int GetWidth() const
		{
			return m_width;
		}

		/// <summary>
		/// 縦を返す
		/// </summary>
		/// <returns></returns>
		int GetHeight() const
		{
			return m_height;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		ViewPort GetViewPort() const
		{
			return m_viewport;
		}
	private:
		ID3D11Texture2D* m_renderTarget = NULL;
		ID3D11RenderTargetView* m_renderTargetView = NULL;
		ID3D11Texture2D* m_depthStencil = NULL;
		ID3D11DepthStencilView* m_depthStencilView = NULL;
		ID3D11ShaderResourceView* m_renderTargetSRV = NULL;
		ID3D11UnorderedAccessView* m_renderTargetUAV = NULL;
		ID3D11Texture2D* m_resolveTextureMSAA = NULL;

		DXGI_FORMAT m_colFormat = DXGI_FORMAT_UNKNOWN;
		DXGI_FORMAT m_dsFormat = DXGI_FORMAT_UNKNOWN;
		ViewPort m_viewport;
		int m_width = 0;
		int m_height = 0;
		bool m_isMSAA = false;
	};
}