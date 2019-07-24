#include "PreCompile.h"
#include "GPUViewer.h"

#include "StructuredBuffer.h"

namespace UsualEngine
{
	ID3D11ShaderResourceView* SRV::Create(StructuredBuffer* structuredBuffer)
	{
		ID3D11ShaderResourceView* srv = NULL;
		ID3D11Buffer* pBuf = structuredBuffer->GetBody();
		if (pBuf != nullptr) {
			D3D11_BUFFER_DESC descBuf;
			ZeroMemory(&descBuf, sizeof(descBuf));
			pBuf->GetDesc(&descBuf);

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
			desc.BufferEx.FirstElement = 0;

			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

			HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateShaderResourceView(pBuf, &desc, &srv);
			if (FAILED(hr)) {
				return false;
			}
		}
		return srv;
	}
	ID3D11ShaderResourceView* SRV::Create(ID3D11Texture2D* texture)
	{
		ID3D11ShaderResourceView* srv = NULL;
		if (texture != nullptr) {
			D3D11_TEXTURE2D_DESC texDesc;
			texture->GetDesc(&texDesc);
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
			ZeroMemory(&SRVDesc, sizeof(SRVDesc));
			SRVDesc.Format = texDesc.Format;
			SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = texDesc.MipLevels;

			HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateShaderResourceView(texture, &SRVDesc, &srv);
			if (FAILED(hr)) {
				return NULL;
			}
		}
		return srv;
	}

	//ID3D11UnorderedAccessView* UAV::Create(StructuredBuffer& structuredBuffer)
	//{
	//	ID3D11UnorderedAccessView* uav = NULL;
	//	ID3D11Buffer* pBuf = structuredBuffer.GetBody();
	//	if (pBuf != nullptr) {
	//		D3D11_BUFFER_DESC descBuf;
	//		ZeroMemory(&descBuf, sizeof(descBuf));
	//		pBuf->GetDesc(&descBuf);

	//		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	//		ZeroMemory(&desc, sizeof(desc));
	//		desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	//		desc.Buffer.FirstElement = 0;

	//		desc.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
	//		desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

	//		HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateUnorderedAccessView(pBuf, &desc, &uav);
	//		if (FAILED(hr)) {
	//			return NULL;
	//		}
	//	}
	//	return uav;
	//}

}

