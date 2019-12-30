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
			/// x:blendMapを使うか
			/// y:texture1を使うか blendのR
			/// z:texture2を使うか blendのB
			/// w:texture3を使うか blendのG
			/// 
			/// yzが0以外の場合はtexture1とtexture2を使う
			/// 全部0の場合デフォルトのテクスチャ
			/// </summary>
			DirectX::XMINT4 isUseTextures;
		};
		void Release() override;
		/// <summary>
		/// DeleteGOされた瞬間呼ばれる
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
		ConstantBuffer m_groundCB;		//定数バッファ
		GroundCB m_goundData;			//定数バッファの中身。
		Shader m_vsShader;
		Shader m_psShader;
		ID3D11ShaderResourceView* m_blendMap = nullptr;
		ID3D11ShaderResourceView* m_textures[3] = {nullptr};
	};
}