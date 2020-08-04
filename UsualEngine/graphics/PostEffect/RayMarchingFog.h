#pragma once


namespace UsualEngine
{
	class PostEffect;

	/// <summary>
	/// 
	/// </summary>
	class RayMarchingFog
	{
	public:
		RayMarchingFog();
		~RayMarchingFog();

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
		/// <param name="pe"></param>
		void Render(PostEffect* pe);
	private:
		//定数バッファの値
		struct CBData
		{
			CMatrix mVP;				//ビュープロジェクション
			CMatrix mVPI;				//逆ビュープロジェクション
			CVector4 mainLightColor;	//メインライトの色。
			CVector3 mainLightDir;		//メインライトの方向。
			float NONE = 0;
			CVector3 camPos;			//カメラの位置。
			float offset;				//オフセット
			float camFar;
			float camNear;
		};
		ConstantBuffer m_cBuffer;		//定数バッファ
		CBData m_cbData;				//定数バッファのデータ
		Shader m_ps;		//ピクセルシェーダ

		RenderTarget m_rtFog;		//フォグ用のRT
		RenderTarget m_rtVolume;	//LightVolume用のRT
		GaussianBlur m_blur;		//ブラー
		
		float m_offsetSpeed = 100.f;	//オフセットスピード
		float m_maxOffset = 1000.f;		//オフセットの最大値。
	};
}