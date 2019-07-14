#pragma once


namespace UsualEngine
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer() {};
		~ConstantBuffer() { Release(); }
		void Release();
		bool Create(const void* data,int size);
	private:
		ID3D11Buffer* mBuffer;
	};
}