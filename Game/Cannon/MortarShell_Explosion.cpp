#include "stdafx.h"
#include "MortarShell_Explosion.h"

void MortarShell_Explosion::Release()
{
	DeleteGO(m_light);
}

void MortarShell_Explosion::OnDestroy()
{
	Release();
	m_time = 0.f;
}

void MortarShell_Explosion::Awake()
{
	m_light = ue::NewGO<ue::LightPoint>(0);
	m_light->SetCol(m_minColor);
	m_light->SetDecay(m_minDecay);
	m_light->SetRadius(m_minRadius);
}

bool MortarShell_Explosion::Start()
{
	return true;
}

void MortarShell_Explosion::Update()
{
	float dtime = ue::gameTime()->GetDeltaTime();
	m_time += dtime;
	

	if (m_time < m_maxPowTime)
	{
		float d = m_minDecay + (m_maxDecay - m_minDecay) * m_time/m_maxPowTime;
		float r = m_minRadius + (m_maxRadius - m_minRadius) * m_time / m_maxPowTime;
		ue::CVector4 col;
		col.Lerp(m_time/ m_maxPowTime, m_minColor, m_maxColor);
		m_light->SetCol(col);
		m_light->SetDecay(d);
		m_light->SetRadius(r);
	}
	else if (m_time < m_endTime)
	{
		float d = m_maxDecay + (m_minDecay - m_maxDecay) * (m_time - m_maxPowTime) / (m_endTime - m_maxPowTime);
		float r = m_maxRadius + (m_minRadius - m_maxRadius) * (m_time - m_maxPowTime) / (m_endTime - m_maxPowTime);
		ue::CVector4 col;
		col.Lerp((m_time - m_maxPowTime) / (m_endTime - m_maxPowTime), m_maxColor, m_minColor);
		m_light->SetCol(col);
		m_light->SetDecay(d);
		m_light->SetRadius(r);
	}
	else
	{
		auto pThis = this;
		ue::DeleteGO(pThis);
	}
}
