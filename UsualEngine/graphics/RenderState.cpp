#include "PreCompile.h"
#include "RenderState.h"

namespace UsualEngine
{
	RenderState::RenderState()
	{
		InitBlendState();
	}
	RenderState::RenderState()
	{
		m_blendStatePreset.disable->Release();
		m_blendStatePreset.trans->Release();
		m_blendStatePreset.add->Release();
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
	ID3D11SamplerState* RenderState::GetSamplerState(int kind)
	{
		switch (kind)
		{
		case ssPoint: return m_samplerStatePreset.pointSampling;
		case ssLiner: return m_samplerStatePreset.linerSampling;
		}
		return nullptr;
	}
}