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
	if (!mIsStart && mIsActive && !mIsDead)
	{
		mIsStart = Start();
	}
}

void UsualEngine::GameObject::WrapUpdate()
{
	
}

void UsualEngine::GameObject::WrapRender()
{
}

void UsualEngine::GameObject::WrapPostRender()
{
}