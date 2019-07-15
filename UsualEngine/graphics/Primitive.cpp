#include "PreCompile.h"
#include "Primitive.h"



namespace UsualEngine
{
	void Primitive::Release()
	{
		mIndexBuffer.Release();
		mVertexBuffer.Release();
	}
	bool Primitive::Cteate(D3D_PRIMITIVE_TOPOLOGY topo, int vertexNum, int stride, const void* vertexData, int indexNum, IndexBuffer::EIndexType indexType, const void* indexData)
	{
		mTopology = topo;
		bool res = mVertexBuffer.Create(vertexNum, stride, vertexData);
		if (!res)
			return false;
		res = mIndexBuffer.Create(indexNum, indexType, indexData);
		if (!res)
			return false;
		return true;
	}

	void Primitive::Draw()
	{
		ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		UINT offset = 0;
		UINT stride = mVertexBuffer.GetStride();
		dc->IASetVertexBuffers(0, 1, &mVertexBuffer.GetBody(), &stride, &offset);

		dc->IASetIndexBuffer
		(
			mIndexBuffer.GetBody(), 
			mIndexBuffer.GetIndexType() == IndexBuffer::e16bit ? DXGI_FORMAT_R16_UINT: DXGI_FORMAT_R32_UINT,
			0
		);

		dc->IASetPrimitiveTopology(mTopology);

		dc->DrawIndexed(mIndexBuffer.GetIndexNum(), 0, 0);
	}
}