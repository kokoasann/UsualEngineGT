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
			float concentration = 0.95f;		//濃さ。

			CVector2 screenOffset = { 0,0 };		//画面のオフセット
			CVector2 screenSize = { 0,0 };
			CMatrix mProjI;
		};
		CBData m_cbData;				//定数バッファデータ
		ConstantBuffer m_constBuffer;	//定数バッファ
		
		CVector3 m_pos;
	};
}