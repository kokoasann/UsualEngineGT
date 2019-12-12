#include "PreCompile.h"
#include "GameObject.h"

UsualEngine::GameObject::GameObject()
{
}

UsualEngine::GameObject::~GameObject()
{
}

void UsualEngine::GameObject::WrapStart()
{
	if (!m_isStartGO && m_isActiveGO && !m_isDeadGO)
	{
		m_isStartGO = Start();
	}
}

void UsualEngine::GameObject::WrapUpdate()
{
	if (m_isStartGO && m_isActiveGO && !m_isDeadGO)
	{
		Update();
	}
}

void UsualEngine::GameObject::WrapRender()
{
	if (m_isStartGO && m_isActiveGO && !m_isDeadGO)
	{
		Render();
	}
}

void UsualEngine::GameObject::WrapPostRender()
{
	if (m_isStartGO && m_isActiveGO && !m_isDeadGO)
	{
		PostRender();
	}
}