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
		void InitG(const wchar_t* path, AnimationClip* anims = nullptr, int animCount = 0, EnFbxUpAxis axis = enFbxUpAxisZ);

		void SetBlendMap(const wchar_t* path);
		void SetTexture(int num,const wchar_t* path);

		void Render();
	private:
		ConstantBuffer m_groundCB;		//�萔�o�b�t�@
		GroundCB m_goundData;			//�萔�o�b�t�@�̒��g�B
		Shader m_vsShader;
		Shader m_psShader;
		ID3D11ShaderResourceView* m_blendMap = nullptr;
		ID3D11ShaderResourceView* m_textures[3] = {nullptr};
	};
}