#pragma once


namespace UsualEngine
{
	class StructuredBuffer;
	//SharderResourceView
	namespace SRV
	{
		/*
		structureBuffer�p
		���s�����NULL��Ԃ�
		*/
		ID3D11ShaderResourceView* Create(StructuredBuffer* structuredBuffer);
		
		/*
		�e�N�X�`���p
		���s�����NULL��Ԃ�
		*/
		ID3D11ShaderResourceView* Create(ID3D11Texture2D* texture);
	}

	////UnorderedAccessView
	//namespace UAV
	//{

	//	ID3D11UnorderedAccessView* Create(StructuredBuffer& structuredBuffer);
	//}

}