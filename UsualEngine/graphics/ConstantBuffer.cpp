#include "PreCompile.h"
#include "ConstantBuffer.h"


namespace UsualEngine
{
	void ConstantBuffer::Release()
	{
		if (mBuffer != NULL)
		{
			mBuffer->Release();
			mBuffer = NULL;
		}
	}
	bool ConstantBuffer::Create(const void* data, int size)
	{
		Release();
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = (((size - 1) / 16) + 1) * 16;	//16バイトアライメントに切りあげる。
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;

		HRESULT hr;
		if (data) {
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = data;
			hr = usualEngine()->GetGraphicsEngine()->GetD3DDevice()->CreateBuffer(&desc, &InitData, &mBuffer);
		}
		else {
			hr = usualEngine()->GetGraphicsEngine()->GetD3DDevice()->CreateBuffer(&desc, NULL, &mBuffer);
		}
		if (FAILED(hr)) {
			return false;
		}
		return true;
	}
}