#pragma once
#include "Character/Character.h"
#include "PlayerCamera.h"
#include "PlayerMotion.h"

class Cannon;

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
	GameObject* m_gmList[PLAYER_COMPONENT_NUM] = { 0 };	//�R���|�[�l���g�B��3�̃C���X�^���X�̃|�C���^�������Ɋi�[����

	ue::AnimationClip m_anim[PA_num];		//�A�j���[�V����
	ue::Pad* m_pad = 0;								//�Q�[���p�b�h
	//bool m_isWalk = false;								//�����Ă�?
	//PlayerAnim m_PlayingAnim = PlayerAnim::PA_idol;	//���Đ����Ă�A�j��
	//float m_animLug = 1.f;
	//float m_animLug_2idle = 0.0f;
	//float m_lugTime = 0.f;
	//ue::CVector3 m_moved = ue::CVector3::AxisZ();
	//ue::CVector3 m_dir = ue::CVector3::AxisZ();
	//float m_oldSpeed = 0.0f;

	//ue::SkinModelRender::MoveFunc m_walkFunc;
	//ue::Bone* m_footL = nullptr;
	//ue::Bone* m_footR = nullptr;
	//bool m_isJustedL = false;
	//bool m_isJustedR = false;
	//float m_justTimeL = 0.f;
	//float m_justTimeR = 0.f;

	float m_HP = 10.f;			//�q�b�g�|�C���g
	float m_AP = 10.f;			//�A�N�V�����|�C���g

	ue::SkinModel m_cannonMesh;			//�L���m���̃R���W�����̍쐬�ɕK�v��SkinModel
	std::vector<Cannon*> m_cannons;		//���o���Ă�L���m���̔z��B
};