#pragma once
#include "Character/Character.h"
#include "Character/CharacterMoveMotion.h"
#include "Character/CharacterRotateMotion.h"

#define ENEMY_MAX_COMPONENT 5
//エネミーのHPとかAPとかの型。
typedef float EnemyStateValue;

class Player;
class Enemy :public ue::GameObject
{
public:
	struct EnemyInitState
	{
		EnemyStateValue HP = 0;
		EnemyStateValue AP = 0;
	};
	Enemy();
	virtual ~Enemy();
	void Init(const EnemyInitState& eis);
	
	void UpdateComponent();
	void AddComponent(ue::GameObject* go)
	{
		if (m_componentCount >= ENEMY_MAX_COMPONENT)
			std::abort();
		m_components[m_componentCount++] = go;
	}
	static void SetPlayer(Player* p)
	{
		m_player = p;
	}
protected:
	Character m_chara;
	CharacterMoveMotion m_charaMove;
	CharacterRotateMotion m_charaRotate;
	ue::GameObject* m_components[ENEMY_MAX_COMPONENT];
	int m_componentCount = 0;

	EnemyStateValue m_HP = 10;
	EnemyStateValue m_AP = 10;

	static Player* m_player;
};