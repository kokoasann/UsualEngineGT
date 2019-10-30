#pragma once

//ガウスブラーに必要な厚みの配列の数
#define BLUR_NUM_WEIGHT 9
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

		/// <summary>
		/// 
		/// </summary>
		void UpdateWeight();
		/// <summary>
		/// 
		/// </summary>
		/// <param name="src"></param>
		/// <param name="primitive"></param>
		/// <returns></returns>
		ID3D11ShaderResourceView* Render(ID3D11ShaderResourceView* src, Primitive* primitive);

		void SetDispersion(float v)
		{
			m_dispersion = v;
			m_isChangeBOKE = true;
		}
		float GetDispersion()
		{
			return m_dispersion;
		}

		/// <summary>
		/// 定数バッファ用の構造体
		/// </summary>
		struct BlurParam
		{
			CVector4 offset = CVector4::Zero();					//
			float weights[BLUR_NUM_WEIGHT] = {0};		//
		};
	private:
		RenderTarget m_renderTargetX;	//レンダーターゲット ｴｪｪｯｸｽｯ!!!
		RenderTarget m_renderTargetY;	//レンダーターゲット ワイ
		Shader m_vsXBlur;			//横用の頂点シェーダ
		Shader m_vsYBlur;			//縦用の頂点シェーダ
		Shader m_psBlur;				//ピクセルシェーダ
		BlurParam m_bp;				//constantBufferに入れるデータ
		ConstantBuffer m_cb;		//定数バッファ
		float m_dispersion = 25.f;	//ボケ具合。
		bool m_isChangeBOKE = true;	//dispersionの値が変わった場合trueになる
	};
}