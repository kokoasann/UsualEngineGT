#pragma once


namespace UsualEngine
{
#if DEBUG_FUNC
	class DebugSwitch
	{
	private:
		DebugSwitch(){}
	public:
		static std::function<void()> nullFunc;
		~DebugSwitch() 
		{
			for (auto s : m_radioButton)
				delete s;
			for (auto s : m_checkButton)
				delete s;
			for (auto it = m_radioBoxs.begin(); it != m_radioBoxs.end(); it++)
			{
				for (auto s : it->second->box)
					delete s;
				delete it->second;
			}
		}

		static DebugSwitch* Instance()
		{
			static DebugSwitch ds;
			return &ds;
		}

		struct SSwitch
		{
			SSwitch(char k1, char k2, std::function<void()>& update)
			{
				key1 = k1;
				key2 = k2;
				updateFunc = update;
			}
			SSwitch(char k1, char k2, std::function<void()>& on, std::function<void()>& off)
			{
				key1 = k1;
				key2 = k2;
				triggerOnFunc = on;
				triggerOffFunc = off;
			}
			SSwitch(char k1, char k2, std::function<void()>& update, std::function<void()>& on, std::function<void()>& off)
			{
				key1 = k1;
				key2 = k2;
				updateFunc = update;
				triggerOnFunc = on;
				triggerOffFunc = off;
			}
			char key1 = 0;
			char key2 = 0;
			bool isPushed = false;
			bool oldFrameIsPushed = false;
			std::function<void()> updateFunc = nullFunc;
			std::function<void()> triggerOnFunc = nullFunc;
			std::function<void()> triggerOffFunc = nullFunc;
		};
		struct SRadioBox
		{
			std::vector<SSwitch*> box;
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
		void NewRadioBox(const char* name)
		{
			m_radioBoxs.insert({ name, new SRadioBox() });
		}
		void AddRadioBoxButton(const char* name, SSwitch* s)
		{
			auto it = m_radioBoxs.find(name);
			it->second->box.push_back(s);
		}
	private:
		std::vector<SSwitch*> m_radioButton;
		std::vector<SSwitch*> m_checkButton;
		std::map<std::string, SRadioBox*> m_radioBoxs;
	};
#endif

	static void DebugSwitchUpdate()
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->Update();
#endif
	}

	static void* DebugSwitchNewSwitch(char k1, char k2, std::function<void()>& update)
	{
#if DEBUG_FUNC
		return new DebugSwitch::SSwitch(k1, k2, update);
#else
		return nullptr;
#endif
	}

	static void* DebugSwitchNewSwitch(char k1, char k2, std::function<void()>& on, std::function<void()>& off)
	{
#if DEBUG_FUNC
		return new DebugSwitch::SSwitch(k1, k2, on,off);
#else
		return nullptr;
#endif
	}

	static void* DebugSwitchNewSwitch(char k1, char k2, std::function<void()>& update, std::function<void()>& on, std::function<void()>& off)
	{
#if DEBUG_FUNC
		return new DebugSwitch::SSwitch(k1, k2, update, on, off);
#else
		return nullptr;
#endif
	}

	static void DebugSwitchAddCheck(void* s)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->AddCheckButton(reinterpret_cast<DebugSwitch::SSwitch*>(s));
#endif
	}

	static void DebugSwitchAddRadio(void* s)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->AddRadioButton(reinterpret_cast<DebugSwitch::SSwitch*>(s));
#endif
	}

	static void DebugSwitchNewRadioBox(const char* name)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->NewRadioBox(name);
#endif
	}

	static void DebugSwitchAddRadioBoxButton(const char* name, void* s)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->AddRadioBoxButton(name, reinterpret_cast<DebugSwitch::SSwitch*>(s));
#endif
	}
}