#include "PreCompile.h"
#include "RayMarchingFog.h"

#include "../ShaderSample.h"
#include "../RenderState.h"

namespace UsualEngine
{
	RayMarchingFog::RayMarchingFog()
	{
	}
	RayMarchingFog::~RayMarchingFog()
	{
		Release();
	}
	void RayMarchingFog::Init()
	{
		m_cBuffer.Create(nullptr, sizeof(m_cbData));
		m_ps.Load("Assets/Shader/RayMarchingFog.fx", "PSMain_RMFog", Shader::EnType::PS);
	}
	void RayMarchingFog::Release()
	{
		m_cBuffer.Release();
		m_ps.Release();
	}
	void RayMarchingFog::Render(PostEffect* pe)
	{
		auto ge = usualEngine()->GetGraphicsEngine();
		auto dc = ge->GetD3DDeviceContext();
		auto& cam = usualEngine()->GetMainCamera();
		auto lig = ge->GetLightManager().GetMainLightDirection();

		if (lig != nullptr)
			m_cbData.mainLightDir = lig->GetDir();

		m_cbData.camFar = cam.GetFar();
		m_cbData.camNear = cam.GetNear();
		m_cbData.camPos = cam.GetPosition();
		m_cbData.mVP.Mul(cam.GetViewMatrix(), cam.GetProjectionMatrix());
		m_cbData.mVPI.Inverse(m_cbData.mVP);

		m_cbData.offset += m_offsetSpeed * gameTime()->GetDeltaTime();
		if (m_cbData.offset >m_maxOffset || m_cbData.offset < 0.f)
		{
			m_offsetSpeed *= -1.f;
		}

		ID3D11BlendState* oldbs[1];
		float oldbf[4] = { 0 };
		unsigned int oldbm = 0;
		dc->OMGetBlendState(oldbs, oldbf, &oldbm);
		dc->OMSetBlendState(BlendState::trans, 0, 0xFFFFFFFF);

		RenderTarget* rts[] = { &pe->GetCurrentRenderTarget() };
		ge->OMSetRenderTarget(1, rts);

		dc->UpdateSubresource(m_cBuffer.GetBody(), 0, 0, &m_cbData, 0, 0);
		dc->PSSetConstantBuffers(enSkinModelCBReg_Generic, 1, &m_cBuffer.GetBody());
		
		dc->VSSetShader((ID3D11VertexShader*)ShaderSample::VS_Copy.GetBody(), 0,0);
		dc->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), 0, 0);
		dc->IASetInputLayout(ShaderSample::VS_Copy.GetInputLayout());

		pe->DrawPrimitive();

		dc->OMSetBlendState(oldbs[0], oldbf, oldbm);
	}
}