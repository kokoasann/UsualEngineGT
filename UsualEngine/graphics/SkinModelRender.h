#pragma once


namespace UsualEngine
{
	class SkinModelRender :public GameObject
	{
	public:
		/// <summary>
		/// IKを用いて行う移動の処理の関数型。
		/// </summary>
		using MoveFunc = std::function<void(CVector3& pos)>;
		/// <summary>
		/// IKを用いて行う回転の処理の関数型。
		/// </summary>
		using RotateFunc = std::function<void(CQuaternion& rot)>;

		/// <summary>
		/// 開放。
		/// </summary>
		void Release();
		/// <summary>
		/// DeleteGOされた瞬間呼ばれる
		/// </summary>
		void OnDestroy() override{}

		/// <summary>
		/// 更新する
		/// </summary>
		void Update() override;
		/// <summary>
		/// 描画する
		/// </summary>
		void Render() override;
		
		/// <summary>
		/// 初期化関数。
		/// </summary>
		/// <param name="path">モデルのファイルパス</param>
		/// <param name="anims">アニメーションクリップ</param>
		/// <param name="animCount">アニメーションクリップの数</param>
		/// <param name="axis">モデルの軸</param>
		void Init(const wchar_t* path, AnimationClip* anims = nullptr, int animCount = 0, EnFbxUpAxis axis = enFbxUpAxisZ);

		/// <summary>
		/// アニメーションを再生する
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号</param>
		/// <param name="interpolateTime">補間時間</param>
		/// <param name="startTime">アニメーションの始まる時間</param>
		void PlayAnim(int animNo, float interpolateTime = 0.f, float startTime = 0.f)
		{
			m_animation.Play(animNo, interpolateTime, startTime);
		}
		
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

		void SetAlphaMap(const wchar_t* path, const wchar_t* materialName);

		/// <summary>
		/// スキンモデルを返す
		/// </summary>
		/// <returns></returns>
		SkinModel& GetSkinModel()
		{
			return m_skinModel;
		}

		/// <summary>
		/// animationをかえす。
		/// </summary>
		/// <returns>保持するanimationを返す</returns>
		Animation& GetAnimation()
		{
			return m_animation;
		}

		/// <summary>
		/// IKの設定をする。
		/// </summary>
		/// <param name="effector">エフェクタボーン</param>
		/// <param name="end">エンドボーン</param>
		/// <param name="radius">コライダーの半径</param>
		IK* SetingIK(Bone* effector, Bone* end, float radius, bool isCreateRigitBody = false)
		{
			return m_animation.SetingIK(effector, end, radius, isCreateRigitBody, m_position);
		}

		/// <summary>
		/// IKを用いて行う移動の処理関数のセット
		/// </summary>
		/// <param name="mf">[](ue::CVector3& pos)->void</param>
		void SetMoveFunc(MoveFunc mf)
		{
			m_moveFunc = mf;
			m_isUseMoveFunc = true;
		}
		/// <summary>
		/// IKを用いて行う回転の処理関数のセット
		/// </summary>
		/// <param name="rf">[](ue::CQuaternion& rot)->void</param>
		void SetRotateFunc(RotateFunc rf)
		{
			m_rotateFunc = rf;
			m_isUseRotateFunc = true;
		}

		/// <summary>
		/// アニメーションの保持する全てのIKの摩擦率をセットする
		/// </summary>
		/// <param name="f">1~0がおすすめ。この範囲外の値を設定すると面白い動きをしてしまう可能性がありますのでご注意ください。</param>
		void SetIKRub(float f,Bone* bone)
		{
			m_animation.SetIKRub(f,bone);
		}

		void SetRefractiveIndex(float f)
		{
			m_skinModel.SetRefractiveIndex(f);
		}
	private:
		SkinModel m_skinModel;	//siknModel
		Animation m_animation;	//Animation

		CVector3 m_position = CVector3::Zero();					//ポジション
		CQuaternion m_rotation = CQuaternion::Identity();		//回転.
		CVector3 m_scale = CVector3::One();						//スケール

		bool m_isAlphaModel = false;
		Shader m_psAlpha;

		bool m_isRenderingOK = false; //レンダリングしてもよかと？

		//↓IK用とか書いてるけど、べつにそんなことはない。
		MoveFunc m_moveFunc;					//IK用の移動関数。
		bool m_isUseMoveFunc=false;				//移動関数を使うか？
		RotateFunc m_rotateFunc;				//IK用の回転関数。
		bool m_isUseRotateFunc = false;			//回転関数を使うか？
	};
}