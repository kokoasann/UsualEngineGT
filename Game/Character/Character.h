#pragma once

class Character
{
public:
	Character();
	~Character();

	void SetMoveFunc(ue::SkinModelRender::MoveFunc movefunc);
private:
	ue::SkinModelRender* m_model = nullptr;
	ue::Bone* m_footL = nullptr;
	ue::Bone* m_footR = nullptr;
	ue::Bone* m_handL = nullptr;
	ue::Bone* m_handR = nullptr;
	ue::CharacterController m_characon;
	ue::AnimationClip* m_animClip = nullptr;

};