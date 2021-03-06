#include "PreCompile.h"
#include "GraphicsEngine.h"
#include "RenderState.h"

#if DEBUG_FUNC
#include "Debug/Debug.h"
#endif

namespace UsualEngine
{
	GraphicsEngine::GraphicsEngine()
	{
	}


	GraphicsEngine::~GraphicsEngine()
	{
		Release();
	}

	void GraphicsEngine::OMSetRenderTarget(int targetCount, RenderTarget* rtlist[])
	{
		ID3D11RenderTargetView* rtvs[RTV_MAX] = {0};
		ID3D11DepthStencilView* dsv = nullptr;
		
		

		if (rtlist != nullptr)
		{
			m_renderTargetCount = targetCount;
			memcpy(m_nowRenderTargets, rtlist, sizeof(RenderTarget*) * targetCount);
			dsv = rtlist[0]->GetDSV();
			for (int i = 0; i < targetCount; i++)
			{
				rtvs[i] = rtlist[i]->GetRTV();
			}
		}
		else
		{
			ZeroMemory(m_nowRenderTargets, sizeof(RenderTarget*) * RTV_MAX);
		}
		m_pd3dDeviceContext->OMSetRenderTargets(targetCount, rtvs, dsv);
		
	}

	void GraphicsEngine::OMGetRenderTargets(int& targetCount, RenderTarget* rtlist[])
	{
		targetCount = m_renderTargetCount;
		memcpy(rtlist, m_nowRenderTargets, sizeof(RenderTarget*) * m_renderTargetCount);
	}

	void GraphicsEngine::BegineRender()
	{
		//float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
														  //描き込み先をバックバッファにする。
		float ClearColor[4] = { 0.f, 0.f, 0.f, 1.0f };

		//m_pd3dDeviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);
		RenderTarget* rt[] = { &m_mainRenderTarget };
		OMSetRenderTarget(1, rt);
		//バックバッファを灰色で塗りつぶす。

		//m_pd3dDeviceContext->ClearRenderTargetView(m_backBuffer, ClearColor);
		//m_pd3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		m_pd3dDeviceContext->ClearRenderTargetView(m_mainRenderTarget.GetRTV(), ClearColor);
		m_pd3dDeviceContext->ClearDepthStencilView(m_mainRenderTarget.GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		m_pd3dDeviceContext->OMSetDepthStencilState(RenderState::Get().GetDepthStencilState(RenderState::dssSceneRender),0);
	}

	void GraphicsEngine::EndRender()
	{

		//バックバッファとフロントバッファを入れ替える。
		m_pSwapChain->Present(2, 0);
	}
	void GraphicsEngine::EndPostEffect()
	{
		ID3D11Texture2D* pBackBuffer = 0;
		m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);

		ID3D11RenderTargetView* rtl[] = { m_backBuffer };
		m_pd3dDeviceContext->OMSetRenderTargets(1, rtl,nullptr);
		//RenderTarget* rtl[] = { &m_mainRenderTarget };
		//OMSetRenderTarget(1, rtl);

		m_pd3dDeviceContext->IASetInputLayout(m_vsCopy.GetInputLayout());
		m_pd3dDeviceContext->PSSetShader((ID3D11PixelShader*)m_psCopy.GetBody(), 0, 0);
		m_pd3dDeviceContext->VSSetShader((ID3D11VertexShader*)m_vsCopy.GetBody(), 0, 0);

		ID3D11ShaderResourceView* srv[] = { m_postEffect.GetCurrentRenderTarget().GetSRV() };
		m_pd3dDeviceContext->PSSetShaderResources(0, 1, srv);
		m_postEffect.DrawPrimitive();

		pBackBuffer->Release();
		srv[0] = nullptr;
		m_pd3dDeviceContext->PSSetShaderResources(0, 1, srv);
	}
	void GraphicsEngine::PreRenderDraw()
	{
		m_preRender.Render();
		//Physics().DebugDraw();
		auto sdw = m_preRender.GetSoftShadow()->Draw();
		auto& gb = m_preRender.GetGBuffer();
		m_preRender.SendDeferrdConstBuffer();
		D3D11_VIEWPORT viewport;
		viewport.Width = FRAME_BUFFER_W;
		viewport.Height = FRAME_BUFFER_H;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		m_pd3dDeviceContext->RSSetViewports(1, &viewport);
		gb.SetGBuffer();
		ID3D11ShaderResourceView* srv[] = { sdw };
		m_pd3dDeviceContext->PSSetShaderResources(enSkinModelSRVReg_Textur_1, 1, &m_speculaGradation);
		//m_pd3dDeviceContext->PSSetShaderResources(enSkinModelSRVReg_GShadowMap, 1, &sdw);

#if DEBUG_FUNC
		switch (Debug::Instance().gbufferDraw)
		{
		case Debug::gdDefault:
			m_pd3dDeviceContext->PSSetShader((ID3D11PixelShader*)m_psDefferd.GetBody(), 0, 0);
			break;
		case Debug::gdDiffuse:
			m_pd3dDeviceContext->PSSetShader((ID3D11PixelShader*)m_psDefferd_Diffuse.GetBody(), 0, 0);
			break;
		case Debug::gdNormal:
			m_pd3dDeviceContext->PSSetShader((ID3D11PixelShader*)m_psDefferd_Normal.GetBody(), 0, 0);
			break;
		case Debug::gdDepth:
			m_pd3dDeviceContext->PSSetShader((ID3D11PixelShader*)m_psDefferd_Depth.GetBody(), 0, 0);
			break;
		case Debug::gdShadow:
			m_pd3dDeviceContext->PSSetShader((ID3D11PixelShader*)m_psDefferd_Shadow.GetBody(), 0, 0);
			break;
		case Debug::gdSpecular:
			m_pd3dDeviceContext->PSSetShader((ID3D11PixelShader*)m_psDefferd_Specular.GetBody(), 0, 0);
			break;
		}
#else
		m_pd3dDeviceContext->PSSetShader((ID3D11PixelShader*)m_psDefferd.GetBody(), 0, 0);
#endif

		m_pd3dDeviceContext->VSSetShader((ID3D11VertexShader*)m_vsDefferd.GetBody(), 0, 0);
		m_pd3dDeviceContext->IASetInputLayout(m_vsDefferd.GetInputLayout());
		m_postEffect.DrawPrimitive();

		gb.UnSetGBuffer();

		//m_lightManager.RenderPointLight();
	}
	void GraphicsEngine::Release()
	{
		if (m_rasterizerState != NULL) {
			m_rasterizerState->Release();
			m_rasterizerState = NULL;
		}
		if (m_depthStencil != NULL) {
			m_depthStencil->Release();
			m_depthStencil = NULL;
		}
		if (m_depthStencilView != NULL) {
			m_depthStencilView->Release();
			m_depthStencilView = NULL;
		}
		if (m_backBuffer != NULL) {
			m_backBuffer->Release();
			m_backBuffer = NULL;
		}
		if (m_pSwapChain != NULL) {
			m_pSwapChain->Release();
			m_pSwapChain = NULL;
		}
		if (m_pd3dDeviceContext != NULL) {
			m_pd3dDeviceContext->Release();
			m_pd3dDeviceContext = NULL;
		}
		if (m_pd3dDevice != NULL) {
			m_pd3dDevice->Release();
			m_pd3dDevice = NULL;
		}
		if (m_spriteBatch != NULL) {
			delete m_spriteBatch;
			m_spriteBatch = NULL;
		}
		if (m_spriteFont != NULL) {
			delete m_spriteFont;
			m_spriteFont = NULL;
		}
	}
	void GraphicsEngine::Init(HWND hWnd)
	{
		//スワップチェインを作成するための情報を設定する。
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;									//スワップチェインのバッファ数。通常は１。
		sd.BufferDesc.Width = (UINT)FRAME_BUFFER_W;			//フレームバッファの幅。
		sd.BufferDesc.Height = (UINT)FRAME_BUFFER_H;		//フレームバッファの高さ。
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//フレームバッファのフォーマット。R8G8B8A8の32bit。
		sd.BufferDesc.RefreshRate.Numerator = 60;			//モニタのリフレッシュレート。(バックバッファとフロントバッファを入れ替えるタイミングとなる。)
		sd.BufferDesc.RefreshRate.Denominator = 1;			//２にしたら30fpsになる。1でいい。
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//サーフェスまたはリソースを出力レンダー ターゲットとして使用します。
		sd.OutputWindow = hWnd;								//出力先のウィンドウハンドル。
		sd.SampleDesc.Count = 1;							//1でいい。
		sd.SampleDesc.Quality = 0;							//MSAAなし。0でいい。
		sd.Windowed = TRUE;									//ウィンドウモード。TRUEでよい。

															//利用するDirectXの機能セット。DirectX10以上に対応しているGPUを利用可能とする。
															//この配列はD3D11CreateDeviceAndSwapChainの引数として使う。
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		//D3Dデバイスとスワップチェインを作成する。
		D3D11CreateDeviceAndSwapChain(
			NULL,											//NULLでいい。
			D3D_DRIVER_TYPE_HARDWARE,						//D3Dデバイスがアクセスするドライバーの種類。
															//基本的にD3D_DRIVER_TYPE_HARDWAREを指定すればよい。
			NULL,											//NULLでいい。
			0,												//０でいい。
			featureLevels,									//D3Dデバイスのターゲットとなる機能セットを指定する。
															//今回のサンプルはDirectX10以上をサポートするので、
															//それらを含むD3D_FEATURE_LEVELの配列を渡す。
			sizeof(featureLevels) / sizeof(featureLevels[0]),	//機能セットの数。
			D3D11_SDK_VERSION,								//使用するDirectXのバージョン。
															//D3D11_SDK_VERSIONを指定すればよい。
			&sd,											//スワップチェインを作成するための情報。
			&m_pSwapChain,									//作成したスワップチェインのアドレスの格納先。
			&m_pd3dDevice,									//作成したD3Dデバイスのアドレスの格納先。
			&m_featureLevel,								//使用される機能セットの格納先。
			&m_pd3dDeviceContext							//作成したD3Dデバイスコンテキストのアドレスの格納先。
		);

		//書き込み先になるレンダリングターゲットを作成。

		ID3D11Texture2D* pBackBuffer = NULL;
		m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
		m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
		//pBackBuffer->Release();

		DXGI_SAMPLE_DESC msaaD;
		ZeroMemory(&msaaD, sizeof(msaaD));
		msaaD.Count = 1;
		msaaD.Quality = 0;
		/*for (int i = 1; i <= 4; i <<= 1)
		{
			UINT Quality;
			if (SUCCEEDED(m_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_D32_FLOAT, i, &Quality)))
			{
				if (0 < Quality)
				{
					msaaD.Count = i;
					msaaD.Quality = Quality - 1;
				}
			}
		}*/
		bool res = m_mainRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_D32_FLOAT,msaaD,NULL,NULL,true);
		//bool res = m_mainRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_D32_FLOAT,msaaD,pBackBuffer,NULL,true);
		if (!res)
			int o = 0+1;
		RenderTarget* rts[] = { &m_mainRenderTarget };
		OMSetRenderTarget(1, rts);
		m_depthStencilView = m_mainRenderTarget.GetDSV();
		

		//pBackBuffer->Release();
		//深度ステンシルビューの作成。
#if 0
		{
			//深度テクスチャの作成。
			D3D11_TEXTURE2D_DESC texDesc;
			ZeroMemory(&texDesc, sizeof(texDesc));
			texDesc.Width = (UINT)FRAME_BUFFER_W;
			texDesc.Height = (UINT)FRAME_BUFFER_H;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.Format = DXGI_FORMAT_D32_FLOAT;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			texDesc.CPUAccessFlags = 0;
			texDesc.MiscFlags = 0;
			m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &m_depthStencil);
			//深度ステンシルビューを作成。
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = texDesc.Format;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			m_pd3dDevice->CreateDepthStencilView(m_depthStencil, &descDSV, &m_depthStencilView);
		}
#endif
		BlendState::Init();
		SamplerState::Init();
		DepthStencilState::Init();
		RasterizerState::Init();
		//ShaderSampleの初期化。
		ShaderSample::Init();

		D3D11_RASTERIZER_DESC desc = {};
		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = true;

		//ラスタライザとビューポートを初期化。
		m_pd3dDevice->CreateRasterizerState(&desc, &m_rasterizerState);

		D3D11_VIEWPORT viewport;
		viewport.Width = FRAME_BUFFER_W;
		viewport.Height = FRAME_BUFFER_H;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		m_pd3dDeviceContext->RSSetViewports(1, &viewport);
		m_pd3dDeviceContext->RSSetState(m_rasterizerState);
		
		m_shadowMap.Init(2048, 2048);
		m_lightManager.Init();

		m_preRender.Init(FRAME_BUFFER_H, FRAME_BUFFER_W);
		m_postEffect.Init();

		m_vsCopy.Load("Assets/shader/copy.fx", "VSMain", Shader::EnType::VS);
		m_psCopy.Load("Assets/shader/copy.fx", "PSMain", Shader::EnType::PS);
		m_vsDefferd.Load("Assets/shader/DefferdShading.fx", "VSMain_Defferd", Shader::EnType::VS);
		m_psDefferd.Load("Assets/shader/DefferdShading.fx", "PSMain_Defferd", Shader::EnType::PS);

#if DEBUG_FUNC
		m_psDefferd_Diffuse.Load("Assets/shader/DefferdShading.fx", "PSMain_Diffuse", Shader::EnType::PS);
		m_psDefferd_Normal.Load("Assets/shader/DefferdShading.fx", "PSMain_Normal", Shader::EnType::PS);
		m_psDefferd_Depth.Load("Assets/shader/DefferdShading.fx", "PSMain_Depth", Shader::EnType::PS);
		m_psDefferd_Shadow.Load("Assets/shader/DefferdShading.fx", "PSMain_Shadow", Shader::EnType::PS);
		m_psDefferd_Specular.Load("Assets/shader/DefferdShading.fx", "PSMain_Specular", Shader::EnType::PS);
#endif

		//std::wstring st = L"Assets/sprite/Deferred_Grad.dds";
		m_speculaGradation =  SpriteDataManager::Get()->Load(L"Assets/sprite/Deferred_Grad.dds");

		//font用のクラスの初期化。
		m_spriteBatch = new DirectX::SpriteBatch(m_pd3dDeviceContext);
		m_spriteFont = new DirectX::SpriteFont(m_pd3dDevice, L"Assets/font/mgothic.spritefont");

		

		InitBackBuffer();
	}
	void GraphicsEngine::InitBackBuffer()
	{
		//書き込み先になるレンダリングターゲットを作成。
		ID3D11Texture2D* pBackBuffer = NULL;
		HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
		if (FAILED(hr)) {
			return ;
		}
		hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
		if (FAILED(hr)) {
			return ;
		}
		return ;
	}
}