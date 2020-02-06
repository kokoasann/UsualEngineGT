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
			DirectX::XMINT4 isUseSpecular;
		};
		struct GroundCB_VS
		{
			int isUseHightMap = 0;
			float hightScale = 1.0f;
		};
		enum TexKind
		{
			tkGrass,//Grassとなっているが、別に草である必要はない。スプラットマップ上での赤色。
			tkTuti,	//上に同じ	スプラットマップ上での緑色。
			tkOther,//スプラットマップ上での青色。
		};

		enum GroundType
		{
			gtDefault,
			gtBlendTow,
			gtBlendThree,
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
		void InitG(const wchar_t* path, AnimationClip* anims = nullptr, int animCount = 0, EnFbxUpAxis axis = enFbxUpAxisZ,GroundType groundType=gtDefault, bool isUseHightMap=false);

		void SetBlendMap(const wchar_t* path);
		void SetTexture(int num,const wchar_t* path,const wchar_t* materialName=nullptr);

		void SetSpecularMap(int num,const wchar_t* path, const wchar_t* materialName);

		void SetHightMap(const wchar_t* path);

		void Render();
	private:
		ConstantBuffer m_groundCB;		//定数バッファ
		GroundCB m_groundData;			//定数バッファの中身。
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