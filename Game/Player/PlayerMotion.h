#pragma once

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

	void Init(Player* player, Character* chara, ue::Camera* cam, ue::AnimationClip* anim,ue::Pad* pad);
	void InitBone(ue::Bone* footL, ue::Bone* footR);

	void Update()override;

	void Move(const ue::CVector2& padStick, PlayerAnim pa, float movespeed, int am);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="delTime">�f���^�^�C��</param>
	void Walk2Idle(float delTime);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="delTime">�f���^�^�C��</param>
	void JustFoot(float delTime);

	void ChangePlayingAnim(PlayerAnim pa);
private:
	Player* m_player = nullptr;
	Character* m_chara = nullptr;					//�O�g�B
	ue::Camera* m_camera = nullptr;
	ue::AnimationClip* m_anim = nullptr;
	ue::Pad* m_pad = nullptr;

	ue::SkinModelRender::MoveFunc m_noneMF;
	ue::SkinModelRender::RotateFunc m_noneRF;
	ue::SkinModelRender::MoveFunc m_walkMF;

	bool m_isWalk = false;
	PlayerAnim m_PlayingAnim = PlayerAnim::PA_idol;
	float m_animLugBase = 0.2f;
	float m_animLug = 0.2f;
	float m_animLug_2idle = 0.0f;
	float m_lugTime = 0.f;
	ue::CVector3 m_moved = ue::CVector3::AxisZ();
	ue::CVector3 m_dir = ue::CVector3::AxisZ();
	float m_oldSpeedBuff = 0.0f;
	float m_oldSpeed = 0.0f;
	enum EChangeWalk
	{
		CW_slow2fast,		//�x���ړ����瑬���ړ��ɕω�����
		CW_fast2slow		//�����ړ�����x���ړ��ɕω�����
	};
	EChangeWalk m_changeWalk = CW_slow2fast;

	ue::Bone* m_footL = nullptr;
	ue::Bone* m_footR = nullptr;
	bool m_isJustedEnd = false;
	bool m_isJustedStart = false;
	float m_justTimeEnd = 0.f;
	float m_justTimeStart = 0.f;
	enum EJustFoot
	{
		JF_footL,		//�������璲������B
		JF_footR		//�E�����璲������B
	};
	EJustFoot m_startJustFoot = JF_footL;
	bool m_isStartJustFoot = false;

	float m_walkFastSpeed = 300.f;
	float m_dushSpeed = 800.f;
};