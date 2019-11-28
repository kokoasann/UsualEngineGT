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
	/// <param name="delTime">デルタタイム</param>
	void Walk2Idle(float delTime);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="delTime">デルタタイム</param>
	void JustFoot(float delTime);

	void ChangePlayingAnim(PlayerAnim pa);
private:
	Player* m_player = nullptr;
	Character* m_chara = nullptr;					//外身。
	ue::Camera* m_camera = nullptr;
	ue::AnimationClip* m_anim = nullptr;
	ue::Pad* m_pad = 0;

	bool m_isWalk = false;
	PlayerAnim m_PlayingAnim = PlayerAnim::PA_idol;
	float m_animLug = 0.2f;
	float m_animLug_2idle = 0.0f;
	float m_lugTime = 0.f;
	ue::CVector3 m_moved = ue::CVector3::AxisZ();
	ue::CVector3 m_dir = ue::CVector3::AxisZ();
	float m_oldSpeedBuff = 0.0f;
	float m_oldSpeed = 0.0f;

	ue::SkinModelRender::MoveFunc m_walkFunc;
	ue::Bone* m_footL = nullptr;
	ue::Bone* m_footR = nullptr;
	bool m_isJustedL = false;
	bool m_isJustedR = false;
	float m_justTimeL = 0.f;
	float m_justTimeR = 0.f;

	float m_walkFastSpeed = 500.f;
	float m_dushSpeed = 800.f;
};