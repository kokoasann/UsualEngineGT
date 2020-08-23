#include "PreCompile.h"
#include "FogEffectObject.h"

UsualEngine::FogEffectObject::FogEffectObject()
{
	m_constBuffer.Create(nullptr, sizeof(m_cbData));
}

UsualEngine::FogEffectObject::~FogEffectObject()
{
}

void UsualEngine::FogEffectObject::Render(RenderTarget* renderTarget)
{
	auto& cam = usualEngine()->GetMainCamera();
	auto ge = usualEngine()->GetGraphicsEngine();
	auto dc = ge->GetD3DDeviceContext();
	auto& gbuf = ge->GetPreRender().GetGBuffer();

	const auto& viewMat = cam.GetViewMatrix();
	const auto& projMat = cam.GetProjectionMatrix();
	
	CVector3 vert[8];//{{0.5, 0, 0.5},{-0.5, 0, 0.5},{0.5, 0, -0.5},{-0.5, 0, -0.5}, {0.5, 1, 0.5},{-0.5, 1, 0.5},{0.5, 1, -0.5},{-0.5, 1, -0.5}}
	vert[0] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, 0.f, m_cbData.size.z * 0.5f };
	vert[1] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, 0.f, m_cbData.size.z * 0.5f };
	vert[2] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, 0.f, m_cbData.size.z * -0.5f };
	vert[3] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, 0.f, m_cbData.size.z * -0.5f };
	vert[4] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, m_cbData.size.y, m_cbData.size.z * 0.5f };
	vert[5] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, m_cbData.size.y, m_cbData.size.z * 0.5f };
	vert[6] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, m_cbData.size.y, m_cbData.size.z * -0.5f };
	vert[7] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, m_cbData.size.y, m_cbData.size.z * -0.5f };

	for (int i = 0; i < 8; i++)
	{
		viewMat.Mul(vert[i]);
		projMat.Mul(vert[i]);
	}
	CVector3 vMax = { -FLT_MAX,-FLT_MAX ,-FLT_MAX };
	CVector3 vMin = { FLT_MAX,FLT_MAX,FLT_MAX };
	for (int i = 0; i < 8; i++)
	{

		vMax.Max(vert[i]);
		vMin.Min(vert[i]);
	}
	if ((vMax.x < 0.f && vMax.y < 0.f) || (vMin.x > 1.f && vMin.y > 1.f))
	{
		return;
	}
	vMax.Min({ 1,1,1 });
	vMin.Max({ 0,0,0 });
	vMax.x *= FRAME_BUFFER_W;
	vMax.y *= FRAME_BUFFER_H;
	vMin.x *= FRAME_BUFFER_W;
	vMin.y *= FRAME_BUFFER_H;
	CVector2 vSize = { vMax.x - vMin.x, vMax.y - vMin.y };
	D3D11_VIEWPORT vp = { vMin.x, vMin.y, vSize.x, vSize.y, 0, 1 };
	dc->RSSetViewports(1,&vp);

	ge->OMSetRenderTarget(1, &renderTarget);

	RenderTarget* gbuffer[1];
	gbuffer[0] = gbuf.GetGBuffer(GBuffer::GB_Depth);
	
	dc->PSSetShaderResources(enSkinModelSRVReg_GDepthMap, 1, &gbuffer[0]->GetSRV());
	
}
