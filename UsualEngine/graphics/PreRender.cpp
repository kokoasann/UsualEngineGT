#include "PreCompile.h"
#include "PreRender.h"


namespace UsualEngine
{
	void PreRender::Init(float h, float w)
	{
		m_gBuffer.Init(h, w);
	}
	void PreRender::Render()
	{
		m_gBuffer.Draw();
	}
}