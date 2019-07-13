#pragma once

namespace UsualEngine
{
	class IndexBuffer
	{
	public:
		//インデックスの型の種類
		enum EIndexType
		{
			e16bit,
			e32bit,
		};

		IndexBuffer() {}
		~IndexBuffer() { Release(); }

		void Release();
		bool Create(int indexNum, EIndexType indexType, const void* indexData);

		ID3D11Buffer* GetBody()
		{
			return mBuffer;
		}
		EIndexType GetIndexType()
		{
			return mIndexType;
		}
		int GetIndexNum()
		{
			return mIndexNum;
		}
	private:
		ID3D11Buffer* mBuffer = nullptr;//buffer
		EIndexType mIndexType = e16bit;//インデックスのタイプ
		int mIndexNum = 0;						//インデックスの数
	};
}