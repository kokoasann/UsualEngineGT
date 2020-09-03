#pragma once
#include "EffectObjectManager.h"

namespace UsualEngine
{
	class FogEffectObject :public EffectObject
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

		void Render(PostEffect* pe);
	private:
		struct VSCBData
		{
			CVector3 effectTip;
			float projW;
			CVector2 screenOffset;
			CVector2 screenSize;
			CMatrix mProjI;
		};
		struct CBData
		{
			CVector4 color = CVector4::White();	//色。
			CVector3 pos = CVector3::Zero();	//位置。
			//CVector3 size = CVector3::One();	//サイズ
			float radius = 100.f;
			CVector3 tip = CVector3::Zero();
			
			float attenuationY = 0.1f;			//Y減衰。
			float attenuationXZ = 0.1f;			//XZ減衰。
			
			float pConcentration = 0.f;   //パーリンノイズの濃度。
			float pDisperse = 0.f;        //パーリンノイズの散乱具合。
			float pScale = 0.1f;           //パーリンノイズのスケール。
			float wConcentration = 0.8f;   //ウォーリーノイズの濃度。
			float wDisperse = 0.1f;        //ウォーリーノイズの散乱具合。
			float wScale = 0.1f;           //ウォーリーノイズのスケール。

			int dummy1;
			CVector3 mainLightDir;
			
			float projW;
			CVector2 screenOffset = { 0,0 };		//画面のオフセット
			CVector2 screenSize = { 0,0 };
			int dummy2;
			CVector3 camPos;
			int dummy3;
			CMatrix mProjI;
			CMatrix mViewI;
		};
		//VSCBData m_vscbData;
		ConstantBuffer m_vsconstBuffer;	//定数バッファ
		CBData m_cbData;				//定数バッファデータ
		ConstantBuffer m_constBuffer;	//定数バッファ
		Shader m_vsMain;
		Shader m_psMain;
		
		CVector3 m_pos;
	};
}