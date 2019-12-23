#include "PreCompile.h"
#include "RenderState.h"

namespace UsualEngine
{
	RenderState::RenderState()
	{
		InitBlendState();
		InitSamplerState();
		InitDepthStencilState();
	}
	RenderState::~RenderState()
	{
		ReleaseBlendState();
		ReleaseSamplerState();
		ReleaseDepthStencilState();
	}

	void RenderState::InitBlendState()
	{
		auto dev = usualEngine()->GetGraphicsEngine()->GetD3DDevice();
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;
		dev->CreateBlendState(&desc, &m_blendStatePreset.add);

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		dev->CreateBlendState(&desc, &m_blendStatePreset.trans);

		desc.RenderTarget[0].BlendEnable = false;
		dev->CreateBlendState(&desc, &m_blendStatePreset.disable);
	}

	void RenderState::ReleaseBlendState()
	{
		m_blendStatePreset.disable->Release();
		m_blendStatePreset.trans->Release();
		m_blendStatePreset.add->Release();
	}

	ID3D11BlendState* RenderState::GetBlendState(int kind)
	{
		switch (kind)
		{
		case bsDisable: return m_blendStatePreset.disable;
		case bsTrans: return m_blendStatePreset.trans;
		case bsAdd: return m_blendStatePreset.add;
		}
		return nullptr;
	}
	void RenderState::InitSamplerState()
	{
		auto dev = usualEngine()->GetGraphicsEngine()->GetD3DDevice();
		{
			CD3D11_DEFAULT def;
			CD3D11_SAMPLER_DESC desc(def);

			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

			dev->CreateSamplerState(&desc, &m_samplerStatePreset.linerSampling);
		}
		{
			CD3D11_DEFAULT def;
			CD3D11_SAMPLER_DESC desc(def);

			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

			dev->CreateSamplerState(&desc, &m_samplerStatePreset.pointSampling);
		}
	}
	void RenderState::ReleaseSamplerState()
	{
		m_samplerStatePreset.linerSampling->Release();
		m_samplerStatePreset.pointSampling->Release();
	}
	ID3D11SamplerState* RenderState::GetSamplerState(int kind)
	{
		switch (kind)
		{
		case ssPoint: return m_samplerStatePreset.pointSampling;
		case ssLiner: return m_samplerStatePreset.linerSampling;
		}
		return nullptr;
	}
	void RenderState::InitDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		ID3D11Device* pd3d = usualEngine()->GetGraphicsEngine()->GetD3DDevice();
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.StencilEnable = false;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		pd3d->CreateDepthStencilState(&desc, &m_depthStencilStatePreset.sceneRender);

		desc.DepthEnable = false;
		pd3d->CreateDepthStencilState(&desc, &m_depthStencilStatePreset.deferredRender);

		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		pd3d->CreateDepthStencilState(&desc, &m_depthStencilStatePreset.spriteRender);
	}
	void RenderState::ReleaseDepthStencilState()
	{
		m_depthStencilStatePreset.sceneRender->Release();
		m_depthStencilStatePreset.deferredRender->Release();
	}
	ID3D11DepthStencilState* RenderState::GetDepthStencilState(DepthStencilStateKind kind)
	{
		switch (kind)
		{
		case dssSceneRender: return m_depthStencilStatePreset.sceneRender;
		case dssDeferredRender: return m_depthStencilStatePreset.deferredRender;
		case dssSpriteRender: return m_depthStencilStatePreset.spriteRender;
		}
		return nullptr;
	}
}