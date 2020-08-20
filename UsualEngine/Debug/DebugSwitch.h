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
		/// <summary>
		/// スイッチの構造体。
		/// keyが2つ設定されているものは同時押しすると反応する。
		/// </summary>
		struct SSwitch
		{
			/// <summary>
			/// 
			/// </summary>
			/// <param name="k1">key1</param>
			/// <param name="k2">key2</param>
			/// <param name="update">有効になっている間呼ばれる関数</param>
			SSwitch(char k1, char k2, std::function<void()>& update)
			{
				key1 = k1;
				key2 = k2;
				updateFunc = update;
			}

			/// <summary>
			/// 
			/// </summary>
			/// <param name="k1">key1</param>
			/// <param name="k2">key2</param>
			/// <param name="on">有効になった瞬間呼ばれる関数</param>
			/// <param name="off">無効になった瞬間呼ばれる関数</param>
			SSwitch(char k1, char k2, std::function<void()>& on, std::function<void()>& off)
			{
				key1 = k1;
				key2 = k2;
				triggerOnFunc = on;
				triggerOffFunc = off;
			}

			/// <summary>
			/// 
			/// </summary>
			/// <param name="k1">key1</param>
			/// <param name="k2">key2</param>
			/// <param name="update">有効になっている間呼ばれる関数</param>
			/// <param name="on">有効になった瞬間呼ばれる関数</param>
			/// <param name="off">無効になった瞬間呼ばれる関数</param>
			SSwitch(char k1, char k2, std::function<void()>& update, std::function<void()>& on, std::function<void()>& off)
			{
				key1 = k1;
				key2 = k2;
				updateFunc = update;
				triggerOnFunc = on;
				triggerOffFunc = off;
			}
			char key1 = 0;		//key1
			char key2 = 0;		//key2
			bool isPushed = false;			//押された
			bool oldFrameIsPushed = false;	//前のフレームに押されていた
			std::function<void()> updateFunc = nullFunc;		//有効になっている間呼ばれる関数。
			std::function<void()> triggerOnFunc = nullFunc;		//有効になった瞬間呼ばれる関数。
			std::function<void()> triggerOffFunc = nullFunc;	//無効になった瞬間呼ばれる関数。
		};
		/// <summary>
		/// ラジオボタンみたいな
		/// </summary>
		struct SRadioBox
		{
			std::vector<SSwitch*> box;		//スイッチ
		};

		/// <summary>
		/// 更新。
		/// </summary>
		void Update();

		/// <summary>
		/// らでぃおボタンの追加。
		/// </summary>
		/// <param name="s"></param>
		void AddRadioButton(SSwitch* s)
		{
			m_radioButton.push_back(s);
		}
		/// <summary>
		/// チェックボタンの追加。
		/// </summary>
		/// <param name="s"></param>
		void AddCheckButton(SSwitch* s)
		{
			m_checkButton.push_back(s);
		}
		/// <summary>
		/// ラジオボックスを追加。
		/// </summary>
		/// <param name="name">ボックスの名前</param>
		void NewRadioBox(const char* name)
		{
			m_radioBoxs.insert({ name, new SRadioBox() });
		}
		/// <summary>
		/// ラジオボックスにボタンを追加。
		/// </summary>
		/// <param name="name">ボックスの名前</param>
		/// <param name="s">スイッチ</param>
		void AddRadioBoxButton(const char* name, SSwitch* s)
		{
			auto it = m_radioBoxs.find(name);
			it->second->box.push_back(s);
		}
	private:
		std::vector<SSwitch*> m_radioButton;			//ラジオボタンたち
		std::vector<SSwitch*> m_checkButton;			//チェックボタンたち
		std::map<std::string, SRadioBox*> m_radioBoxs;	//ラジオボックスたち
	};
#endif
	/// <summary>
	/// スイッチの更新。
	/// </summary>
	static void DebugSwitchUpdate()
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->Update();
#endif
	}

	/// <summary>
	/// スイッチの作成
	/// </summary>
	/// <param name="k1">key1</param>
	/// <param name="k2">key2</param>
	/// <param name="update">ONの時毎フレーム呼ばれる関数</param>
	/// <returns></returns>
	static void* DebugSwitchNewSwitch(char k1, char k2, std::function<void()>& update)
	{
#if DEBUG_FUNC
		return new DebugSwitch::SSwitch(k1, k2, update);
#else
		return nullptr;
#endif
	}

	/// <summary>
	/// スイッチの作成
	/// </summary>
	/// <param name="k1">key1</param>
	/// <param name="k2">key2</param>
	/// <param name="on">ONになった瞬間呼ばれる関数</param>
	/// <param name="off">OFFになった瞬間呼ばれる関数</param>
	/// <returns></returns>
	static void* DebugSwitchNewSwitch(char k1, char k2, std::function<void()>& on, std::function<void()>& off)
	{
#if DEBUG_FUNC
		return new DebugSwitch::SSwitch(k1, k2, on,off);
#else
		return nullptr;
#endif
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="k1">key1</param>
	/// <param name="k2">key2</param>
	/// <param name="update">ONの時毎フレーム呼ばれる関数</param>
	/// /// <param name="on">ONになった瞬間呼ばれる関数</param>
	/// <param name="off">OFFになった瞬間呼ばれる関数</param>
	/// <returns></returns>
	static void* DebugSwitchNewSwitch(char k1, char k2, std::function<void()>& update, std::function<void()>& on, std::function<void()>& off)
	{
#if DEBUG_FUNC
		return new DebugSwitch::SSwitch(k1, k2, update, on, off);
#else
		return nullptr;
#endif
	}

	/// <summary>
	/// チェックボタンの追加。
	/// </summary>
	/// <param name="s">スイッチのポインタ</param>
	static void DebugSwitchAddCheck(void* s)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->AddCheckButton(reinterpret_cast<DebugSwitch::SSwitch*>(s));
#endif
	}

	/// <summary>
	/// ラジオボタン追加。
	/// </summary>
	/// <param name="s">スイッチのポインタ</param>
	static void DebugSwitchAddRadio(void* s)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->AddRadioButton(reinterpret_cast<DebugSwitch::SSwitch*>(s));
#endif
	}

	/// <summary>
	/// ラジオボックスの追加。
	/// </summary>
	/// <param name="name">ボックスの名前</param>
	static void DebugSwitchNewRadioBox(const char* name)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->NewRadioBox(name);
#endif
	}

	/// <summary>
	/// ラジオボックスにボタンを追加。
	/// </summary>
	/// <param name="name">ボックスの名前</param>
	/// <param name="s">ボタンのポインタ</param>
	static void DebugSwitchAddRadioBoxButton(const char* name, void* s)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->AddRadioBoxButton(name, reinterpret_cast<DebugSwitch::SSwitch*>(s));
#endif
	}
}