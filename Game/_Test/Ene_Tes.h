#pragma once

#include "Enemy/Enemy.h"
class Ene_Gib :public Enemy
{
public:
	enum GibAnim
	{
		GA_idol,
		GA_walk,
		GA_rotate,

		GA_num
	};
	Ene_Gib();
	~Ene_Gib();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;

private:
	ue::AnimationClip m_animClip[GA_num];
	ue::Bone* m_neck = nullptr;
};