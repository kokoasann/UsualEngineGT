#pragma once


namespace UsualEngine
{
#if _DEBUG
	class DebugSwitch
	{
	private:
		DebugSwitch(){}
	public:
		~DebugSwitch() 
		{
			for (auto s : m_radioButton)
				delete s;
			for (auto s : m_checkButton)
				delete s;
		}

		static DebugSwitch* Instance()
		{
			static DebugSwitch ds;
			return &ds;
		}

		struct SSwitch
		{
			SSwitch(char k1, char k2, std::function<void()>& func)
			{
				key1 = k1;
				key2 = k2;
				m_func = func;
			}
			char key1 = 0;
			char key2 = 0;
			bool isPushed = false;
			std::function<void()> m_func;
		};

		void Update();

		void AddRadioButton(SSwitch* s)
		{
			m_radioButton.push_back(s);
		}
		void AddCheckButton(SSwitch* s)
		{
			m_checkButton.push_back(s);
		}
	private:
		std::vector<SSwitch*> m_radioButton;
		std::vector<SSwitch*> m_checkButton;

	};
#endif

	static void DebugSwitchUpdate()
	{
#if _DEBUG
		DebugSwitch::Instance()->Update();
#endif
	}
	static DebugSwitch::SSwitch* DebugSwitchNewSwitch(char k1, char k2, std::function<void()>& func)
	{
#if _DEBUG
		return new DebugSwitch::SSwitch(k1, k2,func);
#else
		return nullptr;
#endif
	}

	static void DebugSwitchAddCheck(DebugSwitch::SSwitch* s)
	{
#if _DEBUG
		DebugSwitch::Instance()->AddCheckButton(s);
#endif
	}

	static void DebugSwitchAddRadio(DebugSwitch::SSwitch* s)
	{
#if _DEBUG
		DebugSwitch::Instance()->AddRadioButton(s);
#endif
	}


#define DEBUG_SWITCH_NEW_SWITCH(k1,k2,func) DebugSwitchNewSwitch(k1,k2,func)
#define DEBUG_SWITCH_ADD_CHECK(s) DebugSwitchAddCheck(s)
#define DEBUG_SWITCH_ADD_RADIO(s) DebugSwitchAddRadio(s)

}