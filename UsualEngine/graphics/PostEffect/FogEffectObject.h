#pragma once


namespace UsualEngine
{
	class FogEffectObject
	{
	public:
		FogEffectObject();
		~FogEffectObject();

		void SetPos(const CVector3& v)
		{
			m_pos = v;
		}
		void SetSize(const float v)
		{
			m_cbData.radius = v;
		}
		void SetColor(const CVector4& c)
		{
			m_cbData.color = c;
		}
		void SetAttenuationY(float v)
		{
			m_cbData.attenuationY = v;
		}
		void SetAttenuationXZ(float v)
		{
			m_cbData.attenuationXZ = v;
		}
		void SetConcentration(float v)
		{
			m_cbData.concentration = v;
		}

		void Render(RenderTarget* renderTarget);
	private:
		
		struct CBData
		{
			CVector4 color = CVector4::White();	//色。
			CVector3 pos = CVector3::Zero();	//位置。
			//CVector3 size = CVector3::One();	//サイズ
			float radius = 1.f;
			CVector3 tip = CVector3::Zero();
			
			float attenuationY = 0.1f;			//Y減衰。
			float attenuationXZ = 0.1f;			//XZ減衰。
			
			float pConcentration = 0.4f;   //パーリンノイズの濃度。
			float pDisperse = 0.2f;        //パーリンノイズの散乱具合。
			float pScale = 0.1f;           //パーリンノイズのスケール。
			float wConcentration = 0.2f;   //ウォーリーノイズの濃度。
			float wDisperse = 0.1f;        //ウォーリーノイズの散乱具合。
			float wScale = 0.1f;           //ウォーリーノイズのスケール。

			CVector3 mainLightDir;

			CVector2 screenOffset = { 0,0 };		//画面のオフセット
			CVector2 screenSize = { 0,0 };
			CMatrix mProjI;
		};
		CBData m_cbData;				//定数バッファデータ
		ConstantBuffer m_constBuffer;	//定数バッファ
		Shader m_psMain;
		
		CVector3 m_pos;
	};
}