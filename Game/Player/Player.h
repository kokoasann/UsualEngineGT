#pragma once
#include "Character/Character.h"
#include "PlayerCamera.h"
#include "PlayerMotion.h"

#define PLAYER_COMPONENT_NUM 3
class Player : public ue::GameObject
{
public:
	enum PlayerAnim
	{
		PA_idol,
		PA_walk,
		PA_walkFast,
		PA_dush,

		PA_num
	};

	Player();
	~Player();

	bool Start() override;
	void Update() override;

	/// <summary>
	/// �v���C���[�̃|�W�V������Ԃ�
	/// </summary>
	/// <returns></returns>
	const ue::CVector3& GetPos() const
	{
		return m_chara.GetPos();
	}

private:
	Character m_chara;					//�O�g�B
	PlayerCamera m_camera;				//�J����
	PlayerMotion m_motion;				//�v���C���[�𓮂����N���X�B
	GameObject* m_gmList[PLAYER_COMPONENT_NUM] = { 0 };

	ue::AnimationClip m_anim[PA_num];
	ue::Pad* m_pad = 0;
	bool m_isWalk = false;
	PlayerAnim m_PlayingAnim = PlayerAnim::PA_idol;
	float m_animLug = 1.f;
	float m_animLug_2idle = 0.0f;
	float m_lugTime = 0.f;
	ue::CVector3 m_moved = ue::CVector3::AxisZ();
	ue::CVector3 m_dir = ue::CVector3::AxisZ();
	float m_oldSpeed = 0.0f;

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