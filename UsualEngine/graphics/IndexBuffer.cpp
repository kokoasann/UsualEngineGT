#include "PreCompile.h"
#include "IndexBuffer.h"


namespace UsualEngine
{
	void IndexBuffer::Release()
	{
		mBuffer->Release();
	}
	bool IndexBuffer::Create(int indexNum, EIndexType indexType, const void* indexData)
	{
		Release();

		int stride = 0;
		if (indexType == e16bit) {
			//16bit
			stride = 2;
		}
		else {
			//32bit
			stride = 4;
		}
		mIndexType = indexType;
		mIndexNum = indexNum;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.StructureByteStride = stride;
		bd.ByteWidth = stride * indexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indexData;

		HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateBuffer(&bd, &InitData, &mBuffer);
		if (FAILED(hr)) {
			return false;
		}
		return true;
	}
}