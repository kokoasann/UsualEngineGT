#pragma once
#include "Character/Character.h"
#include "PlayerCamera.h"
class Player : public ue::GameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;

	/// <summary>
	/// プレイヤーのポジションを返す
	/// </summary>
	/// <returns></returns>
	ue::CVector3 GetPos();
private:
	Character m_chara;
	PlayerCamera m_camera;
	GameObject* m_gmList[2] = { 0 };

	ue::AnimationClip m_anim[3];
};