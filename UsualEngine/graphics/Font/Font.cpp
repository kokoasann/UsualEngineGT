#include "PreCompile.h"
#include "Font.h"
#include "../RenderState.h"

namespace UsualEngine
{
	Font::Font()
	{
		auto ge = usualEngine()->GetGraphicsEngine();
		m_spriteBatch = ge->GetSpriteBatch();
		m_spriteFont = ge->GetSpriteFont();
	}
	Font::~Font()
	{
	}

	void Font::Begin()
	{
		m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, m_scaleMat);
	}
	void Font::End()
	{
		m_spriteBatch->End();
		// Ý’è‚ð–ß‚·
		auto dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		dc->OMSetBlendState(BlendState_Trans(),0, 0xFFFFFFFF);
		dc->RSSetState(RasterizerState::spriteRender);
	}
	void Font::Draw(const wchar_t* text, const CVector2& pos, const CVector4& color, float rot, float sca, const CVector2& pivot)
	{
		CVector2 piv = pivot;
		piv.y = 1.0f - piv.y;
		CVector2 position;
		position.x = pos.x + SPRITE_SPACE_SCREEN_W * 0.5f;
		position.y = -pos.y + SPRITE_SPACE_SCREEN_H * 0.5f;

		switch (m_fontDrawType)
		{
		case FT_Frame:
			DrawFrame(text, position, m_fontEffectColor, rot, sca, piv);
			break;
		case FT_Shadow:
			DrawShadow(text, position, m_fontEffectColor, rot, sca, piv);
			break;
		}

		m_spriteFont->DrawString(m_spriteBatch, text, position.vec, color, rot, DirectX::XMFLOAT2(pivot.x,pivot.y), sca, DirectX::SpriteEffects_None);
	}
	void Font::DrawFrame(const wchar_t* text, const CVector2& pos, const CVector4& color, float rot, float sca, const CVector2& pivot)
	{
		CVector2 ofsList[] = {
			{m_fontEffectOffset,0.f},
			{-m_fontEffectOffset,0.f},
			{0.f,m_fontEffectOffset},
			{0.f,-m_fontEffectOffset},
			{m_fontEffectOffset,m_fontEffectOffset},
			{m_fontEffectOffset,-m_fontEffectOffset},
			{-m_fontEffectOffset,-m_fontEffectOffset},
			{-m_fontEffectOffset,m_fontEffectOffset},
		};
		for (const auto& ofs : ofsList)
		{
			m_spriteFont->DrawString(m_spriteBatch, text, (pos+ofs).vec, color, rot, DirectX::XMFLOAT2(pivot.x, pivot.y), sca, DirectX::SpriteEffects_None);
		}
	}
	void Font::DrawShadow(const wchar_t* text, const CVector2& pos, const CVector4& color, float rot, float sca, const CVector2& pivot)
	{
	}
}