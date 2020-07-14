#pragma once

//ガウスブラーに必要な厚みの配列の数
#define BLUR_NUM_WEIGHT 8
namespace UsualEngine
{
	class Primitive;
	class GaussianBlur
	{
	public:
		GaussianBlur();
		~GaussianBlur();
		void Init(int w,int h);
		void Release();
	private:
		/// <summary>
		/// 
		/// </summary>
		void UpdateWeight();
	public:

		/// <summary>
		/// 使った後は、ビューポートの設定を忘れないように。
		/// </summary>
		/// <param name="src">texture</param>
		/// <param name="w">width</param>
		/// <param name="h">height</param>
		/// <param name="primitive">primitive</param>
		/// <returns>blured tex</returns>
		ID3D11ShaderResourceView* Render(ID3D11ShaderResourceView* src ,float w, float h, Primitive* primitive);

		/// <summary>
		/// ぼかし率。
		/// </summary>
		/// <param name="v"></param>
		void SetDispersion(float v)
		{
			m_dispersion = v;
			m_isChangeBOKE = true;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetDispersion()
		{
			return m_dispersion;
		}

	private:
		/// <summary>
		/// 定数バッファ用の構造体
		/// </summary>
		struct BlurParam
		{
			CVector4 offset = CVector4::Zero();					//
			float weights[BLUR_NUM_WEIGHT] = {0};		//
			float rttexRatio = 0;
		};
	private:
		RenderTarget m_renderTargetX;	//レンダーターゲット ｴｪｪｯｸｽｯ!!!
		RenderTarget m_renderTargetY;	//レンダーターゲット ワイ
		Shader m_vsXBlur;			//横用の頂点シェーダ
		Shader m_vsYBlur;			//縦用の頂点シェーダ
		Shader m_psBlur;				//ピクセルシェーダ
		BlurParam m_bp;				//constantBufferに入れるデータ
		ConstantBuffer m_cb;		//定数バッファ
		ID3D11BlendState* m_blendstate = 0;	//ブレンドステート
		float m_dispersion = 25.f;	//ボケ具合。
		bool m_isChangeBOKE = true;	//dispersionの値が変わった場合trueになる
	};
}