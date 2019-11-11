#pragma once

#include "Enemy/Enemy.h"
class Ene_Gib :public Enemy
{
public:
	Ene_Gib();
	~Ene_Gib();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;

private:

};