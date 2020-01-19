#include "PreCompile.h"
#include "DebugSwitch.h"


namespace UsualEngine
{
	void DebugSwitch::Update()
	{
		for (auto s : m_checkButton)
		{
			if (GetAsyncKeyState(s->key1) && s->key2 == 0?true:GetAsyncKeyState(s->key2))
			{
				if(!s->isPushed)
					s->isPushed = true;
				else
					s->isPushed = false;
				break;
			}
		}

		SSwitch* sw = nullptr;
		for (auto s : m_radioButton)
		{
			if (GetAsyncKeyState(s->key1) && s->key2 == 0 ? true : GetAsyncKeyState(s->key2))
			{
				sw = s;
				s->isPushed = true;
				break;
			}
		}
		if (sw != nullptr)
		{
			for (auto s : m_radioButton)
			{
				if (sw != s)
					s->isPushed = false;
			}
		}

		for (auto it = m_radioBoxs.begin(); it != m_radioBoxs.end(); it++)
		{
			SSwitch* sw = nullptr;
			for (auto s : it->second->box)
			{
				if (GetAsyncKeyState(s->key1) && s->key2 == 0 ? true : GetAsyncKeyState(s->key2))
				{
					sw = s;
					s->isPushed = true;
					break;
				}
			}
			if (sw != nullptr)
			{
				for (auto s : it->second->box)
				{
					if (sw != s)
						s->isPushed = false;
				}
			}
		}

		for (auto s : m_checkButton)
		{
			if (s->isPushed)
				s->m_func();
		}
		for (auto s : m_radioButton)
		{
			if (s->isPushed)
				s->m_func();
		}
		for (auto it = m_radioBoxs.begin(); it != m_radioBoxs.end(); it++)
		{
			for (auto s : it->second->box)
				if(s->isPushed)
					s->m_func();
		}
	}
}