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

	/// <summary>
	/// UnorderedAccessViewのための便利な関数たちの入ったネームスペース
	/// </summary>
	namespace UAV
	{
		/// <summary>
		/// StructuredBufferからUAVを作る
		/// </summary>
		/// <param name="structuredBuffer"></param>
		/// <returns>失敗したらNULLを返す</returns>
		ID3D11UnorderedAccessView* Create(StructuredBuffer& structuredBuffer);

		/// <summary>
		/// テクスチャからUAVを作る
		/// </summary>
		/// <param name="texture"></param>
		/// <returns>失敗したらNULLを返す</returns>
		ID3D11UnorderedAccessView* Create(ID3D11Texture2D* texture);
	}

}