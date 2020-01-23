#pragma once


namespace UsualEngine
{
	class Font
	{
	public:
		Font();
		~Font();

		//フォントにつける飾り
		enum FontDrawType
		{
			FT_None,
			FT_Frame,
			FT_Shadow,
		};
		
		/// <summary>
		/// 開始。
		/// </summary>
		void Begin();
		/// <summary>
		/// 終了。
		/// </summary>
		void End();
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="text"></param>
		/// <param name="pos"></param>
		/// <param name="color"></param>
		/// <param name="rot"></param>
		/// <param name="sca"></param>
		/// <param name="pivot"></param>
		void Draw(const wchar_t* text, const CVector2& pos, const CVector4& color, float rot, float sca, const CVector2& pivot = { 0.5f,0.5f });
		/// <summary>
		/// 枠の描画。
		/// </summary>
		/// <param name="text"></param>
		/// <param name="pos"></param>
		/// <param name="color"></param>
		/// <param name="rot"></param>
		/// <param name="sca"></param>
		/// <param name="pivot"></param>
		void DrawFrame(const wchar_t* text, const CVector2& pos, const CVector4& color, float rot, float sca, const CVector2& pivot);
		/// <summary>
		/// 影の描画。(未実装)
		/// </summary>
		/// <param name="text"></param>
		/// <param name="pos"></param>
		/// <param name="color"></param>
		/// <param name="rot"></param>
		/// <param name="sca"></param>
		/// <param name="pivot"></param>
		void DrawShadow(const wchar_t* text, const CVector2& pos, const CVector4& color, float rot, float sca, const CVector2& pivot);

		/// <summary>
		/// 使うフォントを変える。
		/// </summary>
		/// <param name="font"></param>
		void SetFont(DirectX::SpriteFont* font)
		{
			m_spriteFont = font;
		}
		/// <summary>
		/// フォントにつける飾りの色。
		/// </summary>
		/// <param name="color"></param>
		void SetFontEffectColor(const CVector4& color)
		{
			m_fontEffectColor = color;
		}
		/// <summary>
		/// フォントの飾りのオフセット
		/// </summary>
		/// <param name="offset"></param>
		void SetFontEffectOffset(float offset)
		{
			m_fontEffectOffset = offset;
		}

		/// <summary>
		/// 飾りを付けない。
		/// </summary>
		void DrawTypeNone()
		{
			m_fontDrawType = FT_None;
		}
		/// <summary>
		/// 枠を付ける。
		/// </summary>
		void DrawTypeFrame()
		{
			m_fontDrawType = FT_Frame;
		}
		/// <summary>
		/// 影を付ける。(未実装)
		/// </summary>
		void DrawTypeShadow()
		{
			m_fontDrawType = FT_Shadow;
		}
	private:
		DirectX::SpriteBatch* m_spriteBatch = nullptr;		//スプライトバッチ
		DirectX::SpriteFont* m_spriteFont = nullptr;		//フォント
		CMatrix m_scaleMat = CMatrix::Identity();			//
		FontDrawType m_fontDrawType = FT_None;				//つける飾り
		CVector4 m_fontEffectColor = CVector4::CVector4(0.0f, 0.0f, 0.0f, 1.0f);	//飾りの色。
		float m_fontEffectOffset = 5.0f;					//飾りのオフセット
	};
}