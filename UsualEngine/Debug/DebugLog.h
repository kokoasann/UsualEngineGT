#pragma once


namespace UsualEngine
{
	class DebugLog
	{
	public:
		struct Log
		{
			const wchar_t* log = nullptr;
			float lifeTime = 0.0f;
		};
		DebugLog();
		~DebugLog();
		void Print(const wchar_t* str);
		void Render();
	private:
		Font m_font;
		std::list<Log> m_printList;
		CVector2 m_originPos = { -640,360 };
		float m_lifeTime = 10.f;
		float m_fontSize = 0.4f;
		float m_fontSpace = 18.0f;
		CVector4 m_fontColor = CVector4::White();
		int m_logMaxNum = 16;
	};
}