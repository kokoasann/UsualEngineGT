#pragma once

//#define SOFTSHADOW_GAUSS_MAX 
namespace UsualEngine
{
	/// <summary>
	/// (�g���ĂȂ�)
	/// </summary>
	class SoftShadow
	{
	public:
		SoftShadow();
		~SoftShadow();
		void Init();
		ID3D11ShaderResourceView* Draw();
	private:
		RenderTarget m_shadowRT;
		GaussianBlur m_gauss;
		RenderTarget* m_gBufShadow;
	};
}