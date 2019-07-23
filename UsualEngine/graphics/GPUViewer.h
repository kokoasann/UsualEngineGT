#pragma once


namespace UsualEngine
{
	class StructuredBuffer;
	//SharderResourceView
	namespace SRV
	{
		/*
		Ž¸”s‚·‚é‚ÆNULL‚ð•Ô‚·
		*/
		ID3D11ShaderResourceView* Create(StructuredBuffer* structuredBuffer);
		
		ID3D11ShaderResourceView* Create(ID3D11Texture2D* texture);
	}

}