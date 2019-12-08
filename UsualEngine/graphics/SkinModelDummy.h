#pragma once


namespace UsualEngine
{
	class SkinModelDummy :public GameObject
	{
	public:

		/// <summary>
		/// 更新する
		/// </summary>
		void Update() override;

		/// <summary>
		/// 初期化関数。
		/// </summary>
		/// <param name="path">モデルのファイルパス</param>
		/// <param name="anims">アニメーションクリップ</param>
		/// <param name="animCount">アニメーションクリップの数</param>
		/// <param name="axis">モデルの軸</param>
		void Init(const wchar_t* path, EnFbxUpAxis axis = enFbxUpAxisZ);

		/// <summary>
		/// ポジションをセットする
		/// </summary>
		/// <param name="pos"></param>
		void SetPos(const CVector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// ポジションを返す
		/// </summary>
		/// <returns></returns>
		const CVector3& GetPos() const
		{
			return m_position;
		}
		/// <summary>
		/// 回転をセットする
		/// </summary>
		/// <param name="rot"></param>
		void SetRot(const CQuaternion& rot)
		{
			m_rotation = rot;
		}
		/// <summary>
		/// 回転を返す
		/// </summary>
		/// <returns></returns>
		const CQuaternion& GetRot() const
		{
			return m_rotation;
		}
		/// <summary>
		/// スケールをセットする
		/// </summary>
		/// <param name="sca"></param>
		void SetSca(const CVector3& sca)
		{
			m_scale = sca;
		}
		/// <summary>
		/// スケールを返す
		/// </summary>
		/// <returns></returns>
		const CVector3& GetSca() const
		{
			return m_scale;
		}

		/// <summary>
		/// シャドウを出すかどうか
		/// </summary>
		/// <param name="b">出す場合trueを入れる</param>
		void SetIsShadowCaster(bool b)
		{
			m_skinModel.SetIsShadowCaster(b);
		}
		/// <summary>
		/// シャドウ出す？
		/// </summary>
		/// <returns>出す場合trueが返ってくる</returns>
		bool IsShadowCaster()
		{
			return m_skinModel.IsShadowCaster();
		}
		/// <summary>
		/// シャドウを受け取るかどうか
		/// </summary>
		/// <param name="b">受け取る場合trueを入れる</param>
		void SetIsShadowReciever(bool b)
		{
			m_skinModel.SetIsShadowReciever(b);
		}
		/// <summary>
		/// シャドウを受け取る？
		/// </summary>
		/// <returns>受け取る場合trueが返ってくる</returns>
		bool IsShadowReciever()
		{
			return m_skinModel.IsShadowReciever();
		}

		/// <summary>
		/// スキンモデルを返す
		/// </summary>
		/// <returns></returns>
		SkinModel& GetSkinModel()
		{
			return m_skinModel;
		}
	private:
		SkinModel m_skinModel;	//siknModel

		CVector3 m_position = CVector3::Zero();					//ポジション
		CQuaternion m_rotation = CQuaternion::Identity();		//回転.
		CVector3 m_scale = CVector3::One();						//スケール
	};
}