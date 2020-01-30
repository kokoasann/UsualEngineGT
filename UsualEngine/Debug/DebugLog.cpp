#include "PreCompile.h"
#include "DebugLog.h"


namespace UsualEngine
{
	DebugLog::DebugLog()
	{
		m_font.DrawTypeFrame();
	}
	DebugLog::~DebugLog()
	{
	}
	void DebugLog::Print(const wchar_t* str)
	{
		m_printList.push_back({ str,m_lifeTime });
		if (m_printList.size() >= m_logMaxNum)
		{
			m_printList.pop_front();
		}
	}
	void DebugLog::Render()
	{
		if (m_printList.size() == 0)
			return;
		m_font.Begin();
		int count = 0;
		Log logs[32] = { 0 };
		for (auto p : m_printList)
		{
			logs[count++] = p;
		}
		count = 0;
		auto pos = m_originPos;
		for (auto it = &logs[m_printList.size()-1]; count < m_printList.size(); it--, count++)
		{
			m_font.Draw(it->log, pos, m_fontColor, 0.f, m_fontSize, {0.f,1.f});
			pos.y -= m_fontSpace;
		}
		m_font.End();

		float dtime = gameTime()->GetDeltaTime();
		count = 0;
		for (auto& print : m_printList)
		{
			print.lifeTime -= dtime;
			if (print.lifeTime <= 0)
				count++;
		}

		for (int i = 0; i < count; i++)
		{
			m_printList.pop_front();
		}
	}
}