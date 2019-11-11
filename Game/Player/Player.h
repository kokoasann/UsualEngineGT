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
	/// �v���C���[�̃|�W�V������Ԃ�
	/// </summary>
	/// <returns></returns>
	ue::CVector3 GetPos();
private:
	Character m_chara;					//�O�g�B
	PlayerCamera m_camera;				//�J����
	GameObject* m_gmList[2] = { 0 };	

	ue::AnimationClip m_anim[3];
	ue::Pad* m_pad = 0;
	bool m_isWalk = false;

	float m_HP = 10.f;
	float m_AP = 10.f;
};