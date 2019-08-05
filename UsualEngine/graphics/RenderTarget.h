#pragma once


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
			DXGI_SAMPLE_DESC msDesc,ID3D11Texture2D* renderTarget=nullptr,ID3D11Texture2D* depthStencil= nullptr);

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
		ID3D11DepthStencilView* GetDSV()
		{
			return m_depthStencilView;
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
		int m_width = 0;
		int m_height = 0;
		bool m_isMSAA = false;
	};
}