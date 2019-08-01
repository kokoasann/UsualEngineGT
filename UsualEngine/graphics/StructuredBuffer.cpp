#include "PreCompile.h"
#include "StructuredBuffer.h"
#include "GPUViewer.h"

namespace UsualEngine
{
	StructuredBuffer::StructuredBuffer()
	{
	}
	StructuredBuffer::~StructuredBuffer()
	{
	}
	void StructuredBuffer::Release()
	{
		if (mBuffer)
		{
			mBuffer->Release();
			mBuffer = NULL;
		}
	}
	bool StructuredBuffer::Create(const void* pInitData, D3D11_BUFFER_DESC& bufferDesc)
	{
		Release();
		HRESULT hr;
		if (pInitData)
		{
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = pInitData;
			hr = usualEngine()->GetGraphicsEngine()->GetD3DDevice()->CreateBuffer(&bufferDesc, &InitData, &mBuffer);
		}
		else
		{
			hr = usualEngine()->GetGraphicsEngine()->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &mBuffer);

		}
		if (bufferDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			mSRV = SRV::Create(this);
		}
		if(bufferDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{

		}
		return false;
	}
}