#include "stdafx.h"
#include "Enemy.h"


Player* Enemy::m_player = 0;

Enemy::Enemy()
{
	AddComponent(&m_chara);
	AddComponent(&m_charaRotate);
	AddComponent(&m_charaMove);
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
