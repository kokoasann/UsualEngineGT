#pragma once
#include "Character/CharacterMoveMotion.h"
#include "Character/CharacterRotateMotion.h"
#include "PlayerClimb.h"
/// <summary>
/// �v���C���[�̈ړ��֌W�̃N���X
/// </summary>
class Player;
class Character;
class PlayerMotion:public ue::GameObject
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

	PlayerMotion(){}
	~PlayerMotion(){}

	/// <summary>
	/// �J���B
	/// </summary>
	void Release() override{}
	/// <summary>
	/// DeleteGO���ꂽ�u�ԌĂ΂��
	/// </summary>
	void OnDestroy() override{}

	void Init(Player* player, Character* chara, ue::Camera* cam, ue::AnimationClip* anim,ue::Pad* pad);

	void Update()override;

private:
	Player* m_player = nullptr;
	Character* m_chara = nullptr;					//�O�g�B
	ue::Camera* m_camera = nullptr;
	ue::AnimationClip* m_anim = nullptr;
	ue::Pad* m_pad = nullptr;

	CharacterMoveMotion m_charaMove;
	CharacterRotateMotion m_charaRotate;
	PlayerClimb m_playerClimb;

	ue::SkinModelRender::MoveFunc m_noneMF;
	ue::SkinModelRender::RotateFunc m_noneRF;

	bool m_isWalk = false;


	ue::Bone* m_footL = nullptr;
	ue::Bone* m_footR = nullptr;

	bool m_isStartJustFoot = false;

	float m_walkSpeed = 15.f;
	float m_walkFastSpeed = 50.f;
	float m_dushSpeed = 150.f;
};