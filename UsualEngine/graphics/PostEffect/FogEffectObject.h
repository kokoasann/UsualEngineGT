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
			CVector4 color = CVector4::White();	//�F�B
			CVector3 pos = CVector3::Zero();	//�ʒu�B
			//CVector3 size = CVector3::One();	//�T�C�Y
			float radius = 100.f;
			CVector3 tip = CVector3::Zero();
			
			float attenuationY = 0.1f;			//Y�����B
			float attenuationXZ = 0.1f;			//XZ�����B
			
			float pConcentration = 0.f;   //�p�[�����m�C�Y�̔Z�x�B
			float pDisperse = 0.f;        //�p�[�����m�C�Y�̎U����B
			float pScale = 0.1f;           //�p�[�����m�C�Y�̃X�P�[���B
			float wConcentration = 0.8f;   //�E�H�[���[�m�C�Y�̔Z�x�B
			float wDisperse = 0.1f;        //�E�H�[���[�m�C�Y�̎U����B
			float wScale = 0.1f;           //�E�H�[���[�m�C�Y�̃X�P�[���B

			int dummy1;
			CVector3 mainLightDir;
			
			float projW;
			CVector2 screenOffset = { 0,0 };		//��ʂ̃I�t�Z�b�g
			CVector2 screenSize = { 0,0 };
			int dummy2;
			CVector3 camPos;
			int dummy3;
			CMatrix mProjI;
			CMatrix mViewI;
		};
		//VSCBData m_vscbData;
		ConstantBuffer m_vsconstBuffer;	//�萔�o�b�t�@
		CBData m_cbData;				//�萔�o�b�t�@�f�[�^
		ConstantBuffer m_constBuffer;	//�萔�o�b�t�@
		Shader m_vsMain;
		Shader m_psMain;
		
		CVector3 m_pos;
	};
}