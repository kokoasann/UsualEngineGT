#pragma once

namespace UsualEngine
{
	class PostEffect;
	class SSReflection
	{
	public:
		SSReflection();
		virtual ~SSReflection();
		/// <summary>
		/// Initialize
		/// </summary>
		void Init();
		/// <summary>
		/// Release
		/// </summary>
		void Release();

		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="pe"></param>
		void Render(PostEffect* pe);
	private:
		struct CBuffer
		{
			//CMatrix projMat;
			//CMatrix invProjMat;
			CMatrix viewMat;	//カメラビュー行列。
			CMatrix VPMat;		//ビュープロジェクション
			CMatrix invVPMat;	//ビュープロジェクション逆行列
			CVector3 campos;	//カメラの位置
			float camFar;		//遠景距離
			float camNear;		//近景距離
			float rayLen;		//反射を描画する距離
			float rayStep;		//1ステップの距離
		};
		Shader m_vs;		//copy vs
		Shader m_ps;		//SSR ps
		Shader m_copyPS;	//copy ps

		ConstantBuffer m_constBuf;	//定数バッファ値。
		CBuffer m_buf;				//定数バッファ


		RenderTarget m_rtBuffer;	//レンダーターゲット
		GaussianBlur m_blur;		//ブラー
	};
}