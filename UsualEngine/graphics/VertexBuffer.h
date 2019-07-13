#pragma once


namespace UsualEngine
{
	class VertexBuffer
	{
	public:
		VertexBuffer() {}
		~VertexBuffer() { Release(); }
		void Release();
		bool Create(int vertexNum, int stride, const void* vertexData);
		ID3D11Buffer*& GetBody()
		{
			return mBuffer;
		}
		int GetStride()
		{
			return mStride;
		}
	private:
		ID3D11Buffer* mBuffer;	//Buffer
		int mStride;						//これ作るとき使った構造体のサイズ
	};
}