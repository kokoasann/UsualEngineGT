#pragma once
#include "Character/Character.h"

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
	

	static void SetPlayer(Player* p)
	{
		m_player = p;
	}
protected:
	Character m_chara;

	EnemyStateValue m_HP = 10;
	EnemyStateValue m_AP = 10;

	static Player* m_player;
};