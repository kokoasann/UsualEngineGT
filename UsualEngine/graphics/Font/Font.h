#pragma once


namespace UsualEngine
{
	class Font
	{
	public:
		Font();
		~Font();

		//�t�H���g�ɂ������
		enum FontDrawType
		{
			FT_None,
			FT_Frame,
			FT_Shadow,
		};
		
		/// <summary>
		/// �J�n�B
		/// </summary>
		void Begin();
		/// <summary>
		/// �I���B
		/// </summary>
		void End();
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="text"></param>
		/// <param name="pos"></param>
		/// <param name="color"></param>
		/// <param name="rot"></param>
		/// <param name="sca"></param>
		/// <param name="pivot"></param>
		void Draw(const wchar_t* text, const CVector2& pos, const CVector4& color, float rot, float sca, const CVector2& pivot = { 0.5f,0.5f });
		/// <summary>
		/// �g�̕`��B
		/// </summary>
		/// <param name="text"></param>
		/// <param name="pos"></param>
		/// <param name="color"></param>
		/// <param name="rot"></param>
		/// <param name="sca"></param>
		/// <param name="pivot"></param>
		void DrawFrame(const wchar_t* text, const CVector2& pos, const CVector4& color, float rot, float sca, const CVector2& pivot);
		/// <summary>
		/// �e�̕`��B(������)
		/// </summary>
		/// <param name="text"></param>
		/// <param name="pos"></param>
		/// <param name="color"></param>
		/// <param name="rot"></param>
		/// <param name="sca"></param>
		/// <param name="pivot"></param>
		void DrawShadow(const wchar_t* text, const CVector2& pos, const CVector4& color, float rot, float sca, const CVector2& pivot);

		/// <summary>
		/// �g���t�H���g��ς���B
		/// </summary>
		/// <param name="font"></param>
		void SetFont(DirectX::SpriteFont* font)
		{
			m_spriteFont = font;
		}
		/// <summary>
		/// �t�H���g�ɂ������̐F�B
		/// </summary>
		/// <param name="color"></param>
		void SetFontEffectColor(const CVector4& color)
		{
			m_fontEffectColor = color;
		}
		/// <summary>
		/// �t�H���g�̏���̃I�t�Z�b�g
		/// </summary>
		/// <param name="offset"></param>
		void SetFontEffectOffset(float offset)
		{
			m_fontEffectOffset = offset;
		}

		/// <summary>
		/// �����t���Ȃ��B
		/// </summary>
		void DrawTypeNone()
		{
			m_fontDrawType = FT_None;
		}
		/// <summary>
		/// �g��t����B
		/// </summary>
		void DrawTypeFrame()
		{
			m_fontDrawType = FT_Frame;
		}
		/// <summary>
		/// �e��t����B(������)
		/// </summary>
		void DrawTypeShadow()
		{
			m_fontDrawType = FT_Shadow;
		}
	private:
		DirectX::SpriteBatch* m_spriteBatch = nullptr;		//�X�v���C�g�o�b�`
		DirectX::SpriteFont* m_spriteFont = nullptr;		//�t�H���g
		CMatrix m_scaleMat = CMatrix::Identity();			//
		FontDrawType m_fontDrawType = FT_None;				//�������
		CVector4 m_fontEffectColor = CVector4::CVector4(0.0f, 0.0f, 0.0f, 1.0f);	//����̐F�B
		float m_fontEffectOffset = 5.0f;					//����̃I�t�Z�b�g
	};
}