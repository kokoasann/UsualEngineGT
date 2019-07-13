#include "PreCompile.h"
#include "VertexBuffer.h"

namespace UsualEngine
{
	void VertexBuffer::Release()
	{
		mBuffer->Release();
	}
	bool VertexBuffer::Create(int vertexNum, int stride, const void* vertexData)
	{
		Release();
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = stride * vertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertexData;

		HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateBuffer(&bd, &InitData, &mBuffer);
		if (FAILED(hr)) {
			return false;
		}
		mStride = stride;
		return true;
	}
}