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
}


