#include "PreCompile.h"
#include "ShadowMap.h"


namespace UsualEngine
{
	ShadowMap::ShadowMap()
	{
	}
	ShadowMap::~ShadowMap()
	{
	}
	void ShadowMap::Init(float w, float h)
	{
		DXGI_SAMPLE_DESC multiSampleDesc;
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		int iw = w;
		int ih = h;
		
		for (int i = 0; i < MAX_SHADOW_MAP; i++)
		{
			m_shadowMapRT[i].Create(iw, ih, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT, multiSampleDesc);

			m_shadowCBEntity.pixSize[i].x = 1.f / iw;
			m_shadowCBEntity.pixSize[i].y = 1.f / ih;
			iw >>= 1;
			ih >>= 1;
		}
		m_shadowCB.Create(&m_shadowCBEntity, sizeof(m_shadowCBEntity));

		m_lightDirection = { -0.3f,-1,-0.2f };
		m_lightDirection.Normalize();
	}
	void ShadowMap::Update()
	{
		Camera& MainCamera = usualEngine()->GetMainCamera();
		//m_lightHeight = MainCamera.GetPosition().y + 500.f;
		//シーンをレンダリング使用としているカメラを使って、ライトカメラの回転を求める。
		CVector3 cameraDirXZ = MainCamera.GetForward();
		if (fabs(cameraDirXZ.x) < FLT_EPSILON && fabsf(cameraDirXZ.z) < FLT_EPSILON) {
			//ほぼ真上をむいている。
			return;
		}
		/*cameraDirXZ.y = 0.0f;
		cameraDirXZ.Normalize();*/
		//ライトビュー行列の回転成分をを計算する。
		CVector3 lightViewForward = m_lightDirection;
		CVector3 lightViewUp;
		if (fabsf(lightViewForward.y) > 0.999f) {
			//ほぼ真上。
			lightViewUp.Cross(lightViewForward, CVector3::Right());
		}
		else {
			lightViewUp.Cross(lightViewForward, CVector3::Up());
		}
		lightViewUp.Normalize();
		CVector3 lgihtViewRight;
		lgihtViewRight.Cross(lightViewUp, lightViewForward);
		lgihtViewRight.Normalize();

		CMatrix lightViewRot;
		//ライトビューの横を設定する。
		lightViewRot.m[0][0] = lgihtViewRight.x;
		lightViewRot.m[0][1] = lgihtViewRight.y;
		lightViewRot.m[0][2] = lgihtViewRight.z;
		lightViewRot.m[0][3] = 0.0f;
		//ライトビューの上を設定する。
		lightViewRot.m[1][0] = lightViewUp.x;
		lightViewRot.m[1][1] = lightViewUp.y;
		lightViewRot.m[1][2] = lightViewUp.z;
		lightViewRot.m[1][3] = 0.0f;
		//ライトビューの前を設定する。
		lightViewRot.m[2][0] = lightViewForward.x;
		lightViewRot.m[2][1] = lightViewForward.y;
		lightViewRot.m[2][2] = lightViewForward.z;
		lightViewRot.m[2][3] = 0.0f;

		//ライトビューのターゲットを計算。
		CVector3 lightTarget;
		lightTarget = MainCamera.GetPosition();
		lightTarget.y = MainCamera.GetTarget().y;
		lightTarget += cameraDirXZ;
		CVector3 lightPos = lightTarget + m_lightDirection * -m_lightHeight;

		float nearPlaneZ = 0.0f;
		float farPlaneZ;
		CVector3 cameraUp;
		cameraUp.Cross(MainCamera.GetRight(), MainCamera.GetForward());

		float shadowAriaTable[3] = { 0.25f,0.5f,1 };
		//float shadowAriaTable[3] = { 1,0.5f,0.25f };

		//視推台を分割するようにライトビュープロジェクション行列を計算する。
		for (int i = 0; i < MAX_SHADOW_MAP; i++) {
			farPlaneZ = nearPlaneZ + m_lightHeight * shadowAriaTable[i];
			CMatrix mLightView = CMatrix::Identity();
			float halfViewAngle = MainCamera.GetViewAngle() * 0.5f;
			//視推台の8頂点をライト空間に変換してAABBを求めて、正射影の幅と高さを求める。
			float w, h;
			float fFar, fNear;
			CVector3 v[8];
			{
				float t = tan(halfViewAngle);
				CVector3 toUpperNear, toUpperFar;
				toUpperNear = cameraUp * t * nearPlaneZ;
				toUpperFar = cameraUp * t * farPlaneZ;
				t *= MainCamera.GetAspect();
				//近平面の中央座標を計算。
				CVector3 vWk = MainCamera.GetPosition() + cameraDirXZ * nearPlaneZ;
				lightPos = vWk;
				v[0] = vWk + MainCamera.GetRight() * t * nearPlaneZ + toUpperNear;
				v[1] = v[0] - toUpperNear * 2.0f;

				v[2] = vWk + MainCamera.GetRight() * -t * nearPlaneZ + toUpperNear;
				v[3] = v[2] - toUpperNear * 2.0f;

				//遠平面の中央座標を計算。
				vWk = MainCamera.GetPosition() + cameraDirXZ * farPlaneZ;
				lightPos += vWk;
				v[4] = vWk + MainCamera.GetRight() * t * farPlaneZ + toUpperFar;
				v[5] = v[4] - toUpperFar * 2.0f;
				v[6] = vWk + MainCamera.GetRight() * -t * farPlaneZ + toUpperFar;
				v[7] = v[6] - toUpperFar * 2.0f;

				//ライト行列を作成。
				lightPos *= 0.5f;
				lightPos += m_lightDirection * -m_lightHeight;

				mLightView = lightViewRot;
				mLightView.m[3][0] = lightPos.x;
				mLightView.m[3][1] = lightPos.y;
				mLightView.m[3][2] = lightPos.z;
				mLightView.m[3][3] = 1.0f;
				mLightView.Inverse(mLightView);	//ライトビュー完成。
				//視推台を構成する8頂点が計算できたので、ライト空間に座標変換して、AABBを求める。
				CVector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
				CVector3 vMin = { FLT_MAX, FLT_MAX, FLT_MAX };
				for (auto& vInLight : v) {
					mLightView.Mul(vInLight);
					vMax.Max(vInLight);
					vMin.Min(vInLight);

				}
				w = vMax.x - vMin.x;
				h = vMax.y - vMin.y;
			}
			CMatrix proj;
			proj.MakeOrthoProjectionMatrix(
				w,	//ちょい太らせる。
				h,
				m_near,
				m_far
			);
			m_mLVP[i].Mul(mLightView, proj);
			m_shadowCBEntity.mLVP[i] = m_mLVP[i];

			nearPlaneZ = farPlaneZ;
		}
	}
	void ShadowMap::Send2GPU()
	{
		ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		dc->UpdateSubresource(m_shadowCB.GetBody(), 0, NULL, (const SShadowCB*)& m_shadowCBEntity, 0, 0);
		dc->PSSetConstantBuffers(enSkinModelCBReg_Shadow, 1, &m_shadowCB.GetBody());
		for (int i = 0; i < MAX_SHADOW_MAP; i++)
		{
			dc->PSSetShaderResources(enSkinModelSRVReg_ShadowMap + i, 1, &m_shadowMapRT[i].GetSRV());
		}
	}
	void ShadowMap::Render()
	{
		ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		for (int i = 0; i < MAX_SHADOW_MAP; i++)
		{
			ID3D11ShaderResourceView* none[] = { NULL };
			dc->PSSetShaderResources(enSkinModelSRVReg_ShadowMap + i, 1, none);
		}

		RenderTarget* oldRenderTargets[RTV_MAX];
		int numRenderTargetViews;
		usualEngine()->GetGraphicsEngine()->OMGetRenderTargets(numRenderTargetViews, oldRenderTargets);
		for (int i = 0; i < MAX_SHADOW_MAP; i++)
		{
			RenderTarget* rts[MAX_SHADOW_MAP] = { &m_shadowMapRT[i] };
			usualEngine()->GetGraphicsEngine()->OMSetRenderTarget(1, rts);

			/*m_viewPort.Width = (float)m_shadowMapRT[i].GetWidth();
			m_viewPort.Height = (float)m_shadowMapRT[i].GetHeight();
			m_viewPort.TopLeftX = 0.f;
			m_viewPort.TopLeftY = 0.f;
			m_viewPort.MaxDepth = 1.f;
			m_viewPort.MinDepth = 0.f;*/
			dc->RSSetViewports(1, &m_shadowMapRT[i].GetViewPort().Get());

			float col[4] = { 1.f,1.f,1.f,1.f };
			dc->ClearRenderTargetView(m_shadowMapRT[i].GetRTV(), col);
			dc->ClearDepthStencilView(m_shadowMapRT[i].GetDSV(),D3D11_CLEAR_DEPTH,1.f,0);

			for (auto cast : m_shadowCaster)
			{
				cast->Draw(m_mLVP[i], CMatrix::Identity());
			}
		}
		m_shadowCaster.clear();

		usualEngine()->GetGraphicsEngine()->OMSetRenderTarget(numRenderTargetViews, oldRenderTargets);
		m_viewPort.Width = FRAME_BUFFER_W;
		m_viewPort.Height = FRAME_BUFFER_H;
		m_viewPort.TopLeftX = 0.f;
		m_viewPort.TopLeftY = 0.f;
		m_viewPort.MaxDepth = 1.f;
		m_viewPort.MinDepth = 0.f;
		dc->RSSetViewports(1, &m_viewPort);
	}
}