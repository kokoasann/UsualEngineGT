#pragma once
#include "Character/Character.h"

class Player;
class Enemy :public ue::GameObject
{
public:
	Enemy();
	virtual ~Enemy();

private:
	Character m_chara;
	Player* m_player = 0;

	float m_HP = 10.f;
	float m_AP = 10.f;
};