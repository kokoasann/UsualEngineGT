#pragma once


namespace UsualEngine
{
	class StructuredBuffer;
	//SharderResourceView
	namespace SRV
	{
		/*
		失敗するとNULLを返す
		*/
		ID3D11ShaderResourceView* Create(StructuredBuffer* structuredBuffer);
		
		ID3D11ShaderResourceView* Create(ID3D11Texture2D* texture);
	}

}