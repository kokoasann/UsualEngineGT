#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace UsualEngine
{
	class Primitive
	{
	public:
		Primitive() {}
		~Primitive() { Release(); }

		void Release();

		bool Cteate
		(
			D3D_PRIMITIVE_TOPOLOGY topo,
			int vertexNum,
			int stride,
			const void* vertexData,
			int indexNum,
			IndexBuffer::EIndexType indexType,
			const void* indexData
		);

		void Draw();
	private:
		VertexBuffer mVertexBuffer;		//頂点バッファ
		IndexBuffer mIndexBuffer;		//インデックスバッファ

		D3D_PRIMITIVE_TOPOLOGY mTopology; //topology
	};
}