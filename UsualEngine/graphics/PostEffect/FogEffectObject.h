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
			CVector4 color = CVector4::White();	//�F�B
			CVector3 pos = CVector3::Zero();	//�ʒu�B
			//CVector3 size = CVector3::One();	//�T�C�Y
			float radius = 1.f;
			CVector3 tip = CVector3::Zero();
			
			float attenuationY = 0.1f;			//Y�����B
			float attenuationXZ = 0.1f;			//XZ�����B
			float concentration = 0.95f;		//�Z���B

			CVector2 screenOffset = { 0,0 };		//��ʂ̃I�t�Z�b�g
			CVector2 screenSize = { 0,0 };
			CMatrix mProjI;
		};
		CBData m_cbData;				//�萔�o�b�t�@�f�[�^
		ConstantBuffer m_constBuffer;	//�萔�o�b�t�@
		
		CVector3 m_pos;
	};
}