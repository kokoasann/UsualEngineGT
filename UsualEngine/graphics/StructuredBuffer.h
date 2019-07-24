#pragma once


namespace UsualEngine
{
	class StructuredBuffer
	{
	public:
		StructuredBuffer();
		~StructuredBuffer();

		void Release();

		bool Create(const void* pInitData, D3D11_BUFFER_DESC& bufferDesc);


		ID3D11Buffer* GetBody()
		{
			return mBuffer;
		}
		

	private:
		ID3D11Buffer* mBuffer = NULL;
		ID3D11ShaderResourceView* mSRV = NULL;
		//ID3D11UnorderedAccessView* mUAV = NULL;
	};
}