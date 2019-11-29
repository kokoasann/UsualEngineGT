#include "stdafx.h"
#include "Enemy.h"


Player* Enemy::m_player = 0;

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(const EnemyInitState& eis)
{
}

void Enemy::UpdateComponent()
{
	for (auto com : m_components)
	{
		if (com == nullptr)
			break;
		com->Update();
	}
}
