#include "PreCompile.h"
#include "FogEffectObject.h"
#include "graphics/ShaderSample.h"

UsualEngine::FogEffectObject::FogEffectObject()
{
	m_constBuffer.Create(nullptr, sizeof(m_cbData));
	m_psMain.Load("Assets/shader/FogEffect.fx", "PSMain_FogEffect", Shader::EnType::PS);
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
	/*vert[0] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, 0.f, m_cbData.size.z * 0.5f };
	vert[1] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, 0.f, m_cbData.size.z * 0.5f };
	vert[2] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, 0.f, m_cbData.size.z * -0.5f };
	vert[3] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, 0.f, m_cbData.size.z * -0.5f };
	vert[4] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, m_cbData.size.y, m_cbData.size.z * 0.5f };
	vert[5] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, m_cbData.size.y, m_cbData.size.z * 0.5f };
	vert[6] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, m_cbData.size.y, m_cbData.size.z * -0.5f };
	vert[7] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, m_cbData.size.y, m_cbData.size.z * -0.5f };*/
	
	
	//CVector3 center = m_cbData.pos;

	m_cbData.mainLightDir = ge->GetLightManager().GetMainLightDirection()->GetDir();

	m_cbData.pos = m_pos;
	viewMat.Mul(m_cbData.pos);
	CVector3 top = m_cbData.pos + CVector3(m_cbData.radius, m_cbData.radius, 0);
	CVector3 bottom = m_cbData.pos - CVector3(m_cbData.radius, m_cbData.radius, 0);

	CVector3 clipTop = top;
	CVector3 clipBottom = bottom;
	projMat.Mul(clipTop);
	projMat.Mul(clipBottom);
	if ((clipTop.x < 0.f && clipTop.y < 0.f) || (clipBottom.x > 1.f && clipBottom.y > 1.f))
	{
		return;
	}

	m_cbData.screenOffset = CVector2(clipTop.x, clipTop.y);
	CVector2 vSize = { clipTop.x - clipBottom.x, clipTop.y - clipBottom.y };
	m_cbData.screenSize = vSize;
	vSize.x *= FRAME_BUFFER_W;
	vSize.y *= FRAME_BUFFER_H;

	clipTop.x *= FRAME_BUFFER_W;
	clipTop.y *= FRAME_BUFFER_H;
	clipBottom.x *= FRAME_BUFFER_W;
	clipBottom.y *= FRAME_BUFFER_H;
	//CVector2 vSize = { clipTop.x - clipBottom.x, clipTop.y - clipBottom.y };
	D3D11_VIEWPORT vp = { clipTop.x, clipBottom.y, vSize.x, vSize.y, 0, 1 };
	dc->RSSetViewports(1, &vp);

	CVector3 snormal = m_cbData.pos;
	snormal.Normalize();
	m_cbData.tip = m_cbData.pos + snormal * m_cbData.radius;
	projMat.Mul(m_cbData.tip);

	m_cbData.mProjI.Inverse(projMat);

	dc->UpdateSubresource(m_constBuffer.GetBody(), 0, 0, &m_cbData, 0, 0);
	dc->PSSetConstantBuffers(enSkinModelCBReg_Generic, 1, &m_constBuffer.GetBody());
	
	/*
	for (int i = 0; i < 8; i++)
	{
		viewMat.Mul(vert[i]);
		//projMat.Mul(vert[i]);
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
	*/

	ge->OMSetRenderTarget(1, &renderTarget);

	RenderTarget* gbuffer[1];
	gbuffer[0] = gbuf.GetGBuffer(GBuffer::GB_Depth);
	
	dc->PSSetShaderResources(enSkinModelSRVReg_GDepthMap, 1, &gbuffer[0]->GetSRV());
	
	dc->VSSetShader((ID3D11VertexShader*)ShaderSample::VS_Copy.GetBody(), nullptr, 0);
	dc->PSSetShader((ID3D11PixelShader*)m_psMain.GetBody(), nullptr, 0);

	
}
