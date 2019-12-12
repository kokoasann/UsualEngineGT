#pragma once
#include "Character/Character.h"
#include "PlayerCamera.h"
#include "PlayerMotion.h"
#include "Physics/SphereCollider.h"

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

	/// <summary>
	/// �J���B
	/// </summary>
	void Release() override;
	/// <summary>
	/// DeleteGO���ꂽ�u�ԌĂ΂��
	/// </summary>
	void OnDestroy() override;

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

	float m_HP = 10.f;			//�q�b�g�|�C���g
	float m_AP = 10.f;			//�A�N�V�����|�C���g

	ue::SphereCollider m_collider;
	ue::SkinModel m_cannonMesh;			//�L���m���̃R���W�����̍쐬�ɕK�v��SkinModel
	std::vector<Cannon*> m_cannons;		//���o���Ă�L���m���̔z��B

	bool m_isSettingCannon = false;
};