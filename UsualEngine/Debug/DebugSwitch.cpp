#include "PreCompile.h"
#include "DebugSwitch.h"


namespace UsualEngine
{
	std::function<void()> DebugSwitch::nullFunc = []() {};

	void DebugSwitch::Update()
	{
		if (usualEngine()->GetHwnd() != GetForegroundWindow())
			return;

		for (auto s : m_checkButton)
		{
			if (GetAsyncKeyState(s->key1) & 0x8000 && (s->key2 == 0 || (GetAsyncKeyState(s->key2) & 0x8000)))
			{
				if (s->oldFrameIsPushed)
					continue;
				if (!s->isPushed)
				{
					s->isPushed = true;
					s->triggerOnFunc();
				}
				else
				{
					s->isPushed = false;
					s->triggerOffFunc();
				}
				s->oldFrameIsPushed = true;
			}
			else
				s->oldFrameIsPushed = false;
		}

		SSwitch* sw = nullptr;
		for (auto s : m_radioButton)
		{
			if (GetAsyncKeyState(s->key1) & 0x8000 && (s->key2 == 0 || (GetAsyncKeyState(s->key2) & 0x8000)))
			{
				if (s->oldFrameIsPushed)
					continue;
				s->oldFrameIsPushed = true;
				if (sw != nullptr && sw->key2 == 0 && s->key2 != 0)
				{
					sw = s;
					s->isPushed = true;
					break;
				}
				sw = s;
				s->isPushed = true;
				break;
			}
			else
			{
				s->oldFrameIsPushed = false;
			}
		}
		if (sw != nullptr)
		{
			for (auto s : m_radioButton)
			{
				if (sw != s && s->isPushed)
				{
					s->isPushed = false;
					s->triggerOffFunc();
				}
			}
			sw->triggerOnFunc();
		}

		for (auto it = m_radioBoxs.begin(); it != m_radioBoxs.end(); it++)
		{
			SSwitch* sw = nullptr;
			for (auto s : it->second->box)
			{
				if (GetAsyncKeyState(s->key1) & 0x8000 && (s->key2 == 0 || (GetAsyncKeyState(s->key2) & 0x8000)))
				{
					if (s->oldFrameIsPushed)
						continue;
					s->oldFrameIsPushed = true;
					if (sw != nullptr && sw->key2 == 0 && s->key2 != 0)
					{
						sw = s;
						s->isPushed = true;
						break;
					}
					sw = s;
					s->isPushed = true;
					break;
				}
				else
				{
					s->oldFrameIsPushed = false;
				}
			}
			if (sw != nullptr)
			{
				for (auto s : it->second->box)
				{
					if (sw != s && s->isPushed)
					{
						s->isPushed = false;
						s->triggerOffFunc();
					}
				}
				sw->triggerOnFunc();
			}
		}

		for (auto s : m_checkButton)
		{
			if (s->isPushed)
				s->updateFunc();
		}
		for (auto s : m_radioButton)
		{
			if (s->isPushed)
				s->updateFunc();
		}
		for (auto it = m_radioBoxs.begin(); it != m_radioBoxs.end(); it++)
		{
			for (auto s : it->second->box)
				if(s->isPushed)
					s->updateFunc();
		}
	}
}