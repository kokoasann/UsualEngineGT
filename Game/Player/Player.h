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
	Character m_chara;					//外身。
	PlayerCamera m_camera;				//カメラ
	GameObject* m_gmList[2] = { 0 };	

	ue::AnimationClip m_anim[3];
	ue::Pad* m_pad = 0;
	bool m_isWalk = false;

	float m_HP = 10.f;
	float m_AP = 10.f;
};