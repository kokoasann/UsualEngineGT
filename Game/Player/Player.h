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

	void Move(const ue::CVector2& padStick, PlayerAnim pa, float movespeed, Character::ActionMode am);

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
	PlayerAnim m_PlayingAnim = PlayerAnim::PA_idol;
	float m_animLug = 0.2f;
	float m_animLug_2idle = 0.0f;
	float m_lugTime = 0.f;
	ue::CVector3 m_moved = ue::CVector3::AxisZ();
	ue::CVector3 m_dir = ue::CVector3::AxisZ();

	ue::SkinModelRender::MoveFunc m_walkFunc;
	ue::Bone* m_footL = nullptr;
	ue::Bone* m_footR = nullptr;
	bool m_isJustedL = false;
	bool m_isJustedR = false;
	float m_justTimeL = 0.f;
	float m_justTimeR = 0.f;

	float m_HP = 10.f;
	float m_AP = 10.f;
	float m_walkFastSpeed = 500.f;
	float m_dushSpeed = 800.f;
};