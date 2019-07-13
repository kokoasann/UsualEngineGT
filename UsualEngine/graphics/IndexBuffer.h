#pragma once

namespace UsualEngine
{
	class IndexBuffer
	{
	public:
		//�C���f�b�N�X�̌^�̎��
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
		EIndexType mIndexType = e16bit;//�C���f�b�N�X�̃^�C�v
		int mIndexNum = 0;						//�C���f�b�N�X�̐�
	};
}