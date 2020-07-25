#pragma once


namespace UsualEngine
{
	class PostEffect;
	/// <summary>
	/// 板ポリで雲を描画。
	///	レイマーチングを使う為ちょっと重いかも。
	/// </summary>
	class CloudPlane
	{
	public:
		CloudPlane();
		~CloudPlane();

		/// <summary>
		/// 初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// 開放。
		/// </summary>
		void Release();

		/// <summary>
		/// 描画。
		/// </summary>
		void Render(PostEffect* pe);
	private:
		/// <summary>
		/// 定数バッファの構造体。
		/// </summary>
		struct CBData
		{
			CVector3 campos;	//カメラの位置。
			//CVector3 camdir;
			CVector3 mainLightDir;	//メインライトの向き
		};
		ConstantBuffer m_cBuffer;	//定数バッファ
		CBData m_cbData;			//定数バッファのデータ
		ConstantBuffer m_vscBuffer;	//vs用の定数バッファ
		Shader m_vs;				//頂点シェーダ
		Shader m_ps;				//ピクセルシェーダ
		Primitive m_prim;	//描画する板。

		RenderTarget m_rt;

		CVector3 m_pos = {0.f,5000.f,0.f};					//ポジション
		CVector3 m_scale = CVector3::One()*50000.f;				//スケール
		CQuaternion m_rot = CQuaternion::Identity();	//回転。
	};
}