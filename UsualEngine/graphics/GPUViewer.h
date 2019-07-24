#pragma once


namespace UsualEngine
{
	class StructuredBuffer;
	//SharderResourceView
	namespace SRV
	{
		/*
		structureBuffer用
		失敗するとNULLを返す
		*/
		ID3D11ShaderResourceView* Create(StructuredBuffer* structuredBuffer);
		
		/*
		テクスチャ用
		失敗するとNULLを返す
		*/
		ID3D11ShaderResourceView* Create(ID3D11Texture2D* texture);
	}

	////UnorderedAccessView
	//namespace UAV
	//{

	//	ID3D11UnorderedAccessView* Create(StructuredBuffer& structuredBuffer);
	//}

}