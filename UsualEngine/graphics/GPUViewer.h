#pragma once


namespace UsualEngine
{
	class StructuredBuffer;
	//SharderResourceView
	namespace SRV
	{
		/*
		���s�����NULL��Ԃ�
		*/
		ID3D11ShaderResourceView* Create(StructuredBuffer* structuredBuffer);
		
		ID3D11ShaderResourceView* Create(ID3D11Texture2D* texture);
	}

}