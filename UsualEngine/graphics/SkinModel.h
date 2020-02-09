#pragma once

#include "Skeleton.h"
#include "SkinModelEffect.h"

namespace UsualEngine
{
	/*!
	*@brief	FBXの上方向。
	*/
	enum EnFbxUpAxis {
		enFbxUpAxisY,		//Y-up
		enFbxUpAxisZ,		//Z-up
	};
	/*!
	*@brief	スキンモデルクラス。
	*/
	class SkinModel
	{
	public:
		//メッシュが見つかったときのコールバック関数。
		using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
		//マテリアルのコールバック関数。
		using OnFindMaterial = std::function<void(ModelEffect*)>;
		/*!
		*@brief	デストラクタ。
		*/
		~SkinModel();
		/// <summary>
		/// 開放。
		/// </summary>
		void Release();

		/*!
		*@brief	初期化。
		*@param[in]	filePath		ロードするcmoファイルのファイルパス。
		*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
		*/
		void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);

		void InitInstancing(unsigned int maxInstance);
		/*!
		*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
		*@param[in]	position	モデルの座標。
		*@param[in]	rotation	モデルの回転。
		*@param[in]	scale		モデルの拡大率。
		*/
		void UpdateWorldMatrix(const CVector3& position, const CQuaternion& rotation, const CVector3& scale);
		/// <summary>
		/// モデルをワールド座標系に変換するためのワールド行列を更新する。
		/// </summary>
		/// <param name="wmat">ワールドマトリクス</param>
		void UpdateWorldMatrix(const CMatrix& wmat);
		/*!
		*@brief	ボーンを検索。
		*@param[in]		boneName	ボーンの名前。
		*@return	見つかったボーン。見つからなかった場合はnullptrを返します。
		*/
		Bone* FindBone(const wchar_t* boneName)
		{
			int boneId = m_skeleton.FindBoneID(boneName);
			return m_skeleton.GetBone(boneId);
		}
		/*!
		*@brief	モデルを描画。
		*@param[in]	viewMatrix		カメラ行列。
		*  ワールド座標系の3Dモデルをカメラ座標系に変換する行列です。
		*@param[in]	projMatrix		プロジェクション行列。
		*  カメラ座標系の3Dモデルをスクリーン座標系に変換する行列です。
		*/
		void Draw(CMatrix viewMatrix, CMatrix projMatrix);
		/*!
		*@brief	スケルトンの取得。
		*/
		Skeleton& GetSkeleton()
		{
			return m_skeleton;
		}
		/*!
		*@brief	メッシュを検索する。
		*@param[in] onFindMesh		メッシュが見つかったときのコールバック関数
		*/
		void FindMesh(OnFindMesh onFindMesh) const
		{
			for (auto& modelMeshs : m_modelDx->meshes) {
				for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
					onFindMesh(mesh);
				}
			}
		}
		
		/// <summary>
		/// マテリアルを検索する
		/// </summary>
		/// <param name="func">マテリアルのコールバック関数</param>
		void FindMaterial(OnFindMaterial func)
		{
			FindMesh([&](auto& mesh)
			{
					ModelEffect* eff = reinterpret_cast<ModelEffect*>(mesh->effect.get());
					func(eff);
			});
		}
		/*!
		*@brief	SRVのレジスタ番号。
		*/
		enum EnSkinModelSRVReg {
			enSkinModelSRVReg_DiffuseTexture = 0,		//!<ディフューズテクスチャ。
			enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
		};

		/// <summary>
		/// シャドウを出しますか？
		/// </summary>
		/// <param name="b">true or false</param>
		void SetIsShadowCaster(bool b)
		{
			m_isShadowCaster = b;
		}
		/// <summary>
		/// シャドウ出す？
		/// </summary>
		/// <returns></returns>
		bool IsShadowCaster()
		{
			return m_isShadowCaster;
		}

		/// <summary>
		/// シャドウを受け取りますか？
		/// </summary>
		/// <param name="b">true or false</param>
		void SetIsShadowReciever(bool b)
		{
			m_isShadowReciever = b;
		}
		/// <summary>
		/// シャドウ受ける？
		/// </summary>
		/// <returns></returns>
		bool IsShadowReciever()
		{
			return m_isShadowReciever;
		}

		/// <summary>
		/// モデルの上軸を返す
		/// </summary>
		/// <returns></returns>
		EnFbxUpAxis GetFbxUpAxis()
		{
			return m_enFbxUpAxis;
		}

		void SetSpecularMap(const wchar_t* path);

		void SetInstanceData(unsigned int num,const CMatrix& mat)
		{
			m_instancingData[num] = mat;
		}
	private:
		/*!
		*@brief	サンプラステートの初期化。
		*/
		void InitSamplerState();
		/*!
		*@brief	定数バッファの作成。
		*/
		void InitConstantBuffer();
		/*!
		*@brief	スケルトンの初期化。
		*@param[in]	filePath		ロードするcmoファイルのファイルパス。
		*/
		void InitSkeleton(const wchar_t* filePath);

	private:
		//定数バッファ。
		struct SVSConstantBuffer {
			CMatrix mWorld;
			CMatrix mView;
			CMatrix mProj;
			CVector3 camDir;
			int isShadowReciever;
			int isUseSpecularMap = 0;
		};
		EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBXの上方向。
		ID3D11Buffer* m_cb = nullptr;					//!<定数バッファ。
		Skeleton			m_skeleton;						//!<スケルトン。
		CMatrix				m_worldMatrix;					//!<ワールド行列。
		DirectX::Model* m_modelDx;						//!<DirectXTKが提供するモデルクラス。
		ID3D11SamplerState* m_samplerState = nullptr;		//!<サンプラステート。

		ID3D11ShaderResourceView* m_specularMap = nullptr;

		bool m_isShadowCaster = false;					//影を落とす
		bool m_isShadowReciever = true;				//影が落ちる

		bool m_isInstancing = false;
		unsigned int m_maxInstance = 0;
		unsigned int m_numInstance = 0;
		CMatrix* m_instancingData = nullptr;
		StructuredBuffer m_instancingDataSB;
	};

}