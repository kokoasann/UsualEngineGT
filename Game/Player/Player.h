#pragma once
#include "Character/Character.h"
#include "PlayerCamera.h"
class Player : public ue::GameObject
{
public:
	Player();
	~Player();

	enum PlayerAnim
	{
		PA_idol,
		PA_walk,
		PA_walkFast,
		PA_dush,

		PA_num
	};

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

	ue::AnimationClip m_anim[PA_num];
	ue::Pad* m_pad = 0;
	bool m_isWalk = false;

	ue::CVector3 m_dir = ue::CVector3::AxisZ();

	float m_HP = 10.f;
	float m_AP = 10.f;
};