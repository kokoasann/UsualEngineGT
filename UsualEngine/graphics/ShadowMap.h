#pragma once

#include "SkinModel.h"

namespace UsualEngine
{
	/// <summary>
	/// シャドウマップ
	/// </summary>
	class ShadowMap
	{
	public:
		ShadowMap();
		~ShadowMap();
		
		void Init(float w, float h);

		void Update();

		void Send2GPU();

		void Render();

		void AddShadowCaster(SkinModel* m)
		{
			m_shadowCaster.push_back(m);
		}
	private:
		struct SShadowCB
		{
			CMatrix mLVP[MAX_SHADOW_MAP] = { CMatrix::Identity() };
			CVector4 texoffset[MAX_SHADOW_MAP];
			CVector4 pixSize[MAX_SHADOW_MAP];
			CVector4 depthoffset;
		};
		CVector3 m_lightDirection = CVector3::Down();				//ライトの方向
		float m_near = 0.1f;
		float m_far = 100000.f;
		float m_lightHeight = 10000.f;
		RenderTarget m_shadowMapRT[MAX_SHADOW_MAP];
		ConstantBuffer m_shadowCB;
		std::vector<SkinModel*> m_shadowCaster;
		CMatrix m_mLVP[MAX_SHADOW_MAP] = {CMatrix::Identity()};
		SShadowCB m_shadowCBEntity;
		D3D11_VIEWPORT m_viewPort;
	};
}