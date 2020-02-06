#pragma once

#include "graphics/SkinModelRender.h"

namespace UsualEngine
{
	class SkinModelRender4Ground:public SkinModelRender
	{
	public:
		struct GroundCB
		{
			CVector4 scale = CVector4::Zero();
			CMatrix dir = CMatrix::Identity();
			/// <summary>
			/// x:blendMap���g����
			/// y:texture1���g���� blend��R
			/// z:texture2���g���� blend��B
			/// w:texture3���g���� blend��G
			/// 
			/// yz��0�ȊO�̏ꍇ��texture1��texture2���g��
			/// �S��0�̏ꍇ�f�t�H���g�̃e�N�X�`��
			/// </summary>
			DirectX::XMINT4 isUseTextures;
			DirectX::XMINT4 isUseSpecular;
		};
		struct GroundCB_VS
		{
			int isUseHightMap = 0;
			float hightScale = 1.0f;
		};
		enum TexKind
		{
			tkGrass,//Grass�ƂȂ��Ă��邪�A�ʂɑ��ł���K�v�͂Ȃ��B�X�v���b�g�}�b�v��ł̐ԐF�B
			tkTuti,	//��ɓ���	�X�v���b�g�}�b�v��ł̗ΐF�B
			tkOther,//�X�v���b�g�}�b�v��ł̐F�B
		};

		enum GroundType
		{
			gtDefault,
			gtBlendTow,
			gtBlendThree,
		};
		void Release() override;
		/// <summary>
		/// DeleteGO���ꂽ�u�ԌĂ΂��
		/// </summary>
		void OnDestroy() override{}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="path"></param>
		/// <param name="anims"></param>
		/// <param name="animCount"></param>
		/// <param name="axis"></param>
		void InitG(const wchar_t* path, AnimationClip* anims = nullptr, int animCount = 0, EnFbxUpAxis axis = enFbxUpAxisZ,GroundType groundType=gtDefault, bool isUseHightMap=false);

		void SetBlendMap(const wchar_t* path);
		void SetTexture(int num,const wchar_t* path,const wchar_t* materialName=nullptr);

		void SetSpecularMap(int num,const wchar_t* path, const wchar_t* materialName);

		void SetHightMap(const wchar_t* path);

		void Render();
	private:
		ConstantBuffer m_groundCB;		//�萔�o�b�t�@
		GroundCB m_groundData;			//�萔�o�b�t�@�̒��g�B
		ConstantBuffer m_groundCB_VS;
		GroundCB_VS m_groundVSData;
		Shader m_vsShader;
		Shader m_psShader;
		ID3D11ShaderResourceView* m_blendMap = nullptr;
		ID3D11ShaderResourceView* m_textures[3] = {nullptr};
		ID3D11ShaderResourceView* m_specularMap[3] = { nullptr };
		ID3D11ShaderResourceView* m_hightMap = nullptr;

		GroundType m_groundType = gtDefault;
		bool m_isUseHightMap = false;

		std::wstring m_grassName = L"m_dun";
		std::wstring m_tutiName = L"m_gaia";
		std::wstring m_otherName = L"m_";
	};
}