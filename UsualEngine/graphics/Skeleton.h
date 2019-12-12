/*!
 *@brief	スケルトン
 */
#pragma once

namespace UsualEngine
{
	/*!
	*@brief	ボーン。
	*/
	class Bone {
	public:
		/*!
		*@brief	コンストラクタ。
		*@param[in]	boneName		ボーンの名前。
		*@param[in]	bindPose		バインドポーズの行列。
		*@param[in] invBindPose		バインドポーズの逆行列。
		*@param[in]	parentId		親のボーン番号。
		*@param[in] boneId			ボーン番号。
		*/
		Bone(
			const wchar_t* boneName,
			const CMatrix& bindPose,
			const CMatrix& invBindPose,
			int parentId,
			int boneId
		) :
			m_boneName(boneName),
			m_bindPose(bindPose),
			m_invBindPose(invBindPose),
			m_parentId(parentId),
			m_worldMatrix(bindPose),
			m_boneId(boneId)
		{
		}
		/*!
		 *@brief	ローカル行列を設定
		 */
		void SetLocalMatrix(const CMatrix& m)
		{
			m_localMatrix = m;
		}
		/*!
		 *@brief	ローカル行列を取得。
		 */
		const CMatrix& GetLocalMatrix() const
		{
			return m_localMatrix;
		}
		/*!
		*@brief	ワールド行列を設定。
		*/
		void SetWorldMatrix(const CMatrix& m)
		{
			m_worldMatrix = m;
		}
		/*!
		 *@brief	ワールド行列を取得。
		 */
		const CMatrix& GetWorldMatrix() const
		{
			return m_worldMatrix;
		}
		/*!
		*@brief	バインドポーズの行列を取得。
		*/
		const CMatrix& GetBindPoseMatrix() const
		{
			return m_bindPose;
		}
		/*!
		 *@brief	バインドポーズの逆行列を取得。
		 */
		const CMatrix& GetInvBindPoseMatrix() const
		{
			return m_invBindPose;
		}
		/*!
		 *@brief	親のボーン行列を取得。
		 */
		int GetParentId() const
		{
			return m_parentId;
		}
		/*!
		 *@brief	ボーン番号の取得。
		 */
		int GetNo() const
		{
			return m_boneId;
		}

		/*!
		*@brief	子供を追加。
		*/
		void AddChild(Bone* bone)
		{
			m_children.push_back(bone);
			bone->SetParent(this);
		}
		/*!
		*@brief	子供を取得。
		*/
		const std::vector<Bone*>& GetChildren() const
		{
			return m_children;
		}

		void SetParent(Bone* chil)
		{
			m_parent = chil;
		}
		Bone* GetParent() const
		{
			return m_parent;
		}

		/*!
		 *@brief	名前の取得。
		 */
		const wchar_t* GetName() const
		{
			return m_boneName.c_str();
		}

		bool IsONGround() const
		{
			return m_isONGround;
		}
		void SetIsONGround(bool b)
		{
			m_isONGround = b;
		}


		void SetMove(CVector3 v)
		{
			m_move = v;
		}
		CVector3 GetMove() const
		{
			return m_move;
		}
		void SetAnimMove(CVector3 v)
		{
			m_animMove = v;
		}
		CVector3 GetAnimMove() const
		{
			return m_animMove;
		}

		void SetBaseWorldMatrix(const CMatrix& mat)
		{
			m_oldWorldMatrix = m_baseWorldMatrix;
			m_baseWorldMatrix = mat;
			
		}
		const CMatrix& GetBaseWorldMatrix() const
		{
			return m_baseWorldMatrix;
		}

		void SetOldWorldMatrix(const CMatrix& mat)
		{
			m_oldWorldMatrix = mat;
		}
		const CMatrix& GetOldWorldMatrix() const
		{
			return m_oldWorldMatrix;
		}

		void SetMomentum(const CVector3& v)
		{
			m_momentum = v;
		}
		const CVector3& GetMomentum() const
		{
			return m_momentum;
		}

		/*!
		*@brief	このボーンのワールド空間での位置と回転とスケールを計算する。
		*@param[out]	trans		平行移動量の格納先。
		*@param[out]	rot			回転量の格納先。
		*@param[out]	scale		拡大率の格納先。
		*/
		void CalcWorldTRS(CVector3& trans, CQuaternion& rot, CVector3& scale);

	private:

		std::wstring	m_boneName;
		int				m_parentId = -1;	//!<親のボーン番号。
		int				m_boneId = -1;		//!<ボーン番号。
		CMatrix			m_bindPose;		//!<バインドポーズ。
		CMatrix			m_invBindPose;	//!<バインドポーズの逆行列。
		CMatrix			m_localMatrix = CMatrix::Identity();	//!<ローカル行列。
		CMatrix			m_worldMatrix = CMatrix::Identity();	//!<ワールド行列。
		CVector3		m_positoin = CVector3::Zero();			//!<このボーンのワールド空間での位置。最後にCalcWorldTRSを実行したときの結果が格納されている。
		CVector3		m_scale = CVector3::One();				//!<このボーンの拡大率。最後にCalcWorldTRSを実行したときの結果が格納されている。
		CQuaternion		m_rotation = CQuaternion::Identity();	//!<このボーンの回転。最後にCalcWorldTRSを実行したときの結果が格納されている。
		std::vector<Bone*>	m_children;		//!<子供。
		Bone* m_parent = nullptr;			//親ボーン

		CMatrix			m_baseWorldMatrix = CMatrix::Identity();
		CMatrix			m_oldWorldMatrix = CMatrix::Identity();

		CVector3 m_animMove = CVector3::Zero();
		CVector3 m_move = CVector3::Zero();			//ボーンの移動ベクトル
		CVector3 m_momentum = CVector3::Zero();		//勢い。
		bool m_isONGround = false;
	};
	/*!
	 *@brief	スケルトン。
	 */
	class Skeleton {
	public:
		using OnPostProcessSkeletonUpdate = std::function<void()>;

		Skeleton();
		~Skeleton();

		void Release();

		/*!
		 *@brief	ボーンのローカル行列を設定。
		 *@param[in]	boneNo		ボーン番号
		 *@param[in]	m			行列。
		 */
		void SetBoneLocalMatrix(int boneNo, const CMatrix& m)
		{
			if (boneNo > (int)m_bones.size() - 1) {
				return;
			}
			m_bones[boneNo]->SetLocalMatrix(m);
		}
		/*!
		 *@brief	ボーンの数を取得。
		 */
		int GetNumBones() const
		{
			return static_cast<int>(m_bones.size());
		}
		/*!
		*@brief	ロード。
		*@param[in]	filePath	ファイルパス。
		*/
		bool Load(const wchar_t* filePath);
		/*!
		 *@brief	ボーンの名前からボーンIDを検索。
		 */
		int FindBoneID(const wchar_t* boneName) const
		{
			for (int i = 0; i < (int)m_bones.size(); i++) {
				if (wcscmp(m_bones[i]->GetName(), boneName) == 0) {
					return i;
				}
			}
			//見つからなかった。
			return -1;
		}
		/*!
		*@brief	ボーンを取得。
		*/
		Bone* GetBone(int boneNo)
		{
			return m_bones[boneNo];
		}
		/// <summary>
		/// 全ボーンを取得。
		/// </summary>
		/// <returns></returns>
		std::vector<Bone*>& GetAllBone()
		{
			return m_bones;
		}
		/*!
		*@brief	ボーン行列の配列をGPUに転送。
		*/
		void SendBoneMatrixArrayToGPU();
	public:
		/*!
		*@brief	ボーン行列のストラクチャードバッファを初期化。
		*/
		void InitBoneMatrixArrayStructuredBuffer();
		/*!
		*@brief	ボーン行列のシェーダーリソースビューを初期化。
		*/
		void InitBoneMatrixArrayShaderResourceView();
		/*!
		 *@brief	更新。
		 */
		void Update(const CMatrix& mWorld);

		void UpdateBase(const CMatrix& mWorld);
		/*!
		*@brief	ボーンのワールド行列の更新関数。
		*@details
		* 通常はユーザーがこの関数を呼び出す必要はありません。
		*@param[in]	bone		更新するボーン。
		*@param[in]	parentMatrix	親のボーンのワールド行列。
		*/
		void UpdateBoneWorldMatrix(Bone& bone, const CMatrix& parentMatrix);

		void UpdateBoneBaseWorldMatrix(Bone& bone, const CMatrix& parentMatrix);
	private:

		std::vector<Bone*>			m_bones;					//!<ボーンの配列。
		std::vector<CMatrix>		m_boneMatrixs;				//!<ボーン行列。
		ID3D11Buffer* m_boneMatrixSB = nullptr;	//!<ボーン行列のストラクチャーバッファ。
		ID3D11ShaderResourceView* m_boneMatrixSRV = nullptr;	//!<ボーン行列のSRV。

		CMatrix m_worldMatrix = CMatrix::Identity();
		CMatrix m_oldWorldMatrix = CMatrix::Identity();
	};
}