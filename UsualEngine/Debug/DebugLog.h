#pragma once


namespace UsualEngine
{
	class DebugLog
	{
	public:
		struct Log
		{
			std::wstring log;
			//const wchar_t* log = nullptr;
			float lifeTime = 0.0f;
		};
		DebugLog();
		~DebugLog();
		void Print(const wchar_t* str);
		void Render();
	private:
		Font m_font;												//フォント
		std::list<Log> m_printList;							//表示するログのリスト
		CVector2 m_originPos = { -640,360 };			//ログの表示位置。
		float m_lifeTime = 10.f;								//ログの生存時間。
		float m_firstFontSize = 0.7f;						//一番上に表示されるフォントのサイズ
		float m_firstFontSpace = 28.f;						//一番上に表示されるフォントの間隔。
		float m_fontSize = 0.4f;								//フォントのサイズ
		float m_fontSpace = 18.0f;							//フォントの間隔。
		CVector4 m_fontColor = CVector4::White();	//フォントの色。
		int m_logMaxNum = 16;								//ログの表示される最大数。
	};
}