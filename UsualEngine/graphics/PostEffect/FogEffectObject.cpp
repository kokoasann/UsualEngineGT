#include "PreCompile.h"
#include "FogEffectObject.h"
#include "graphics/ShaderSample.h"

UsualEngine::FogEffectObject::FogEffectObject()
{
	m_vsconstBuffer.Create(nullptr, sizeof(VSCBData));
	m_constBuffer.Create(nullptr, sizeof(m_cbData));
	m_psMain.Load("Assets/shader/FogEffect.fx", "PSMain_FogEffect", Shader::EnType::PS);
	m_vsMain.Load("Assets/shader/FogEffect.fx", "VSMain", Shader::EnType::VS);
}

UsualEngine::FogEffectObject::~FogEffectObject()
{
	
}

void UsualEngine::FogEffectObject::Render(PostEffect* pe)
{
	auto& cam = usualEngine()->GetMainCamera();
	auto ge = usualEngine()->GetGraphicsEngine();
	auto dc = ge->GetD3DDeviceContext();
	auto& gbuf = ge->GetPreRender().GetGBuffer();

	const auto& viewMat = cam.GetViewMatrix();
	const auto& projMat = cam.GetProjectionMatrix();

	m_cbData.camPos = cam.GetPosition();

	VSCBData vcbData;
	vcbData.projW = viewMat.m[3][2];
	m_cbData.projW = viewMat.m[3][2];

	m_cbData.mViewI.Mul(viewMat,projMat);
	m_cbData.mViewI.Inverse(m_cbData.mViewI);

	m_cbData.pos = m_pos;
	
	//CVector3 vert[8];//{{0.5, 0, 0.5},{-0.5, 0, 0.5},{0.5, 0, -0.5},{-0.5, 0, -0.5}, {0.5, 1, 0.5},{-0.5, 1, 0.5},{0.5, 1, -0.5},{-0.5, 1, -0.5}}
	/*vert[0] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, 0.f, m_cbData.size.z * 0.5f };
	vert[1] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, 0.f, m_cbData.size.z * 0.5f };
	vert[2] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, 0.f, m_cbData.size.z * -0.5f };
	vert[3] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, 0.f, m_cbData.size.z * -0.5f };
	vert[4] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, m_cbData.size.y, m_cbData.size.z * 0.5f };
	vert[5] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, m_cbData.size.y, m_cbData.size.z * 0.5f };
	vert[6] = m_cbData.pos + CVector3{ m_cbData.size.x * 0.5f, m_cbData.size.y, m_cbData.size.z * -0.5f };
	vert[7] = m_cbData.pos + CVector3{ m_cbData.size.x * -0.5f, m_cbData.size.y, m_cbData.size.z * -0.5f };*/
	
	
	//CVector3 center = m_cbData.pos;

	auto mainLight = ge->GetLightManager().GetMainLightDirection();
	if (mainLight != nullptr)
		m_cbData.mainLightDir = mainLight->GetDir();
	else
		m_cbData.mainLightDir = { 0,-1,0 };

	CVector4 vpos = m_pos;
	vpos.w = 1.f;
	
	viewMat.Mul(vpos);
	CVector4 top = vpos + CVector4(m_cbData.radius, m_cbData.radius, 0,0);
	CVector4 bottom = vpos - CVector4(m_cbData.radius, m_cbData.radius, 0,0);
	//m_cbData.pos = CVector3(vpos.x, vpos.y, vpos.z);

	CVector4 clipTop = top;
	CVector4 clipBottom = bottom;
	//clipTop.w = 1.f;
	//clipBottom.w = 1.f;
	projMat.Mul(clipTop);
	projMat.Mul(clipBottom);
	clipTop = clipTop / clipTop.w;
	clipBottom = clipBottom / clipBottom.w;

	clipTop.x += 1.f;
	clipTop.y += 1.f;
	clipTop.z += 1.f;
	clipBottom.x += 1.f;
	clipBottom.y += 1.f;
	clipBottom.z += 1.f;
	clipTop = clipTop * 0.5f;
	clipBottom = clipBottom * 0.5f;
	

	clipTop.x = min(clipTop.x, 1);
	clipTop.y = min(clipTop.y, 1);
	clipBottom.x = min(clipBottom.x, 1);
	clipBottom.y = min(clipBottom.y, 1);

	//clipTop.x = 1.f - clipTop.x;
	clipTop.y = 1.f - clipTop.y;
	//clipBottom.x = 1.f - clipBottom.x;
	clipBottom.y = 1.f - clipBottom.y;

	auto buf = clipTop.y;
	clipTop.y = clipBottom.y;
	clipBottom.y = buf;

	if ((clipTop.x < 0.f && clipTop.y < 0.f) || (clipBottom.x > 1.f && clipBottom.y > 1.f) || clipTop.z > 1.f || clipBottom.z > 1.f)
	{
		return;
	}

	CVector2 vSize = { clipTop.x - clipBottom.x, clipTop.y - clipBottom.y };
	
	vSize.x *= FRAME_BUFFER_W;
	vSize.y *= FRAME_BUFFER_H;

	clipTop.x *= FRAME_BUFFER_W;
	clipTop.y *= FRAME_BUFFER_H;
	clipBottom.x *= FRAME_BUFFER_W;
	clipBottom.y *= FRAME_BUFFER_H;
	
	
	//CVector2 vSize = { clipTop.x - clipBottom.x, clipTop.y - clipBottom.y };
	D3D11_VIEWPORT vp = { clipBottom.x, clipBottom.y, vSize.x, vSize.y, 0, 1 };
	dc->RSSetViewports(1, &vp);

	m_cbData.screenSize = vSize;
	m_cbData.screenOffset = CVector2(clipBottom.x, clipBottom.y);
	vcbData.screenSize = vSize;
	vcbData.screenOffset = CVector2(clipBottom.x, clipBottom.y);

	//auto vpos3 = CVector3{ vpos.x,vpos.y ,vpos.z };
	//CVector3 snormal = vpos3;
	//snormal.Normalize();
	
	//snormal *= -m_cbData.radius;
	//CVector4 tip = vpos + CVector4{ snormal.x,snormal.y,snormal.z,0.f };
	//tip.w = 1.f;


	auto norm = m_cbData.camPos - m_pos;
	norm.Normalize();
	norm = m_pos + norm * m_cbData.radius;
	CVector4 tip = CVector4{ norm.x,norm.y,norm.z,1.f };
	viewMat.Mul(tip);

	//m_cbData.tip = vpos3 + (snormal * -m_cbData.radius);
	//vcbData.effectTip = vpos3 + (snormal * -m_cbData.radius);

	projMat.Mul(tip);
	//projMat.Mul(vcbData.effectTip);
	tip = tip / tip.w;
	m_cbData.tip = CVector3{ tip.x,tip.y,tip.z };

	m_cbData.mProjI.Inverse(projMat);
	vcbData.mProjI.Inverse(projMat);

	dc->UpdateSubresource(m_constBuffer.GetBody(), 0, 0, &m_cbData, 0, 0);
	dc->PSSetConstantBuffers(enSkinModelCBReg_Generic, 1, &m_constBuffer.GetBody());
	
	dc->UpdateSubresource(m_vsconstBuffer.GetBody(), 0, 0, &vcbData, 0, 0);
	dc->VSSetConstantBuffers(enSkinModelCBReg_Generic+1, 1, &m_vsconstBuffer.GetBody());
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

	RenderTarget* oldrt[8];
	int oldrtnum;
	ge->OMGetRenderTargets(oldrtnum, oldrt);

	RenderTarget* rt = &pe->GetCurrentRenderTarget();
	ge->OMSetRenderTarget(1, &rt);

	RenderTarget* gbuffer[1];
	gbuffer[0] = gbuf.GetGBuffer(GBuffer::GB_Depth);
	
	dc->PSSetShaderResources(enSkinModelSRVReg_GDepthMap, 1, &gbuffer[0]->GetSRV());
	
	dc->VSSetShader((ID3D11VertexShader*)m_vsMain.GetBody(), nullptr, 0);
	dc->IASetInputLayout(m_vsMain.GetInputLayout());
	dc->PSSetShader((ID3D11PixelShader*)m_psMain.GetBody(), nullptr, 0);

	pe->DrawPrimitive();


	vp = { 0, 0, FRAME_BUFFER_W, FRAME_BUFFER_H, 0, 1 };
	dc->RSSetViewports(1, &vp);
	ge->OMSetRenderTarget(oldrtnum, oldrt);
}
