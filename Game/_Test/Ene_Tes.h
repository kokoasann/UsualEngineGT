#pragma once

#include "Enemy/Enemy.h"
class Ene_GibTes :public Enemy
{
public:
	enum GibAnim
	{
		GA_idol,

		GA_num
	};
	Ene_GibTes();
	~Ene_GibTes();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;

private:
	ue::AnimationClip m_animClip[GA_num];
	ue::Bone* m_neck = nullptr;
};