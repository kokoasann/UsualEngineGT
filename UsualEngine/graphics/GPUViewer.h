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

	/// <summary>
	/// UnorderedAccessView�̂��߂֗̕��Ȋ֐������̓������l�[���X�y�[�X
	/// </summary>
	namespace UAV
	{
		/// <summary>
		/// StructuredBuffer����UAV�����
		/// </summary>
		/// <param name="structuredBuffer"></param>
		/// <returns>���s������NULL��Ԃ�</returns>
		ID3D11UnorderedAccessView* Create(StructuredBuffer& structuredBuffer);

		/// <summary>
		/// �e�N�X�`������UAV�����
		/// </summary>
		/// <param name="texture"></param>
		/// <returns>���s������NULL��Ԃ�</returns>
		ID3D11UnorderedAccessView* Create(ID3D11Texture2D* texture);
	}

}