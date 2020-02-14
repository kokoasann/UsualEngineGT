#pragma once
#include "GBuffer.h"
#include "PostEffect/SoftShadow.h"

namespace UsualEngine
{
	class PreRender
	{
	public:
		PreRender(){}
		~PreRender(){}
		void Init(float h,float w);
		void Render();
		void PrePostRender();
		GBuffer& GetGBuffer()
		{
			return m_gBuffer;
		}
		SoftShadow* GetSoftShadow()
		{
			return &m_softShadow;
		}
		void SendDeferrdConstBuffer();

		void AddPrePostRenderObject(GameObject* go)
		{
			m_prePostRenderObject.push_back(go);
		}

		RenderTarget* GetPrePostRenderTarget()
		{
			return &m_prePostRenderTarget;
		}
	private:
		struct ConstantBufferData
		{
			CMatrix mViewProj = CMatrix::Identity();
			CVector3 camDir = CVector3::Zero();
		};
		GBuffer m_gBuffer;
		SoftShadow m_softShadow;
		ConstantBufferData m_cbData;
		ConstantBuffer m_constBuffer;

		RenderTarget m_prePostRenderTarget;
		std::vector<GameObject*> m_prePostRenderObject;
	};
}