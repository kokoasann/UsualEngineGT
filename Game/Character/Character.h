#pragma once

class Character:public ue::GameObject
{
public:
	enum ActionMode
	{
		AM_Move,
		AM_Rotate,
	};
	enum BoneKind
	{
		BK_None,
		BK_FootL,
		BK_FootR,
		BK_HandL,
		BK_HandR,
		BK_WaistL,
		BK_WaistR,
	};

	Character();
	~Character();
	using wstr = const wchar_t*;

	void Init(ue::SkinModelRender* smr,float ccradius,float ccheight,const ue::CVector3& offset=ue::CVector3::Zero());
	void InitAnimData(ue::AnimationClip* animclip);
	void InitBoneData
	(
		wstr footLname = nullptr,
		wstr footRname = nullptr,
		wstr handLname = nullptr,
		wstr handRname = nullptr,
		wstr waistLname = nullptr,
		wstr waistRname = nullptr
	);

	ue::Bone* FindBone(wstr name,BoneKind bk=BK_None,bool isSetingIK=false,int len=0,float radius=10.f);

	void Update();

	void SetMoveFunc(const ue::SkinModelRender::MoveFunc& movefunc);
	void SetRotateFunc(const ue::SkinModelRender::RotateFunc& rotatefunc);

	void AddEventListener(const ue::Animation::EventListener& ev);

	void ModeMove();
	void ModeRotate();

	const ue::CVector3& GetPos();
	const ue::CQuaternion& GetRot();
private:
	ue::SkinModelRender* m_model = nullptr;
	ue::Bone* m_footL = nullptr;
	ue::Bone* m_footR = nullptr;
	ue::Bone* m_handL = nullptr;
	ue::Bone* m_handR = nullptr;
	ue::Bone* m_waistL = nullptr;
	ue::Bone* m_waistR = nullptr;
	std::vector<ue::Bone*> m_boneList;
	ue::CharacterController m_characon;
	ue::CVector3 m_ccOffset;
	ue::AnimationClip* m_animClip = nullptr;
	ActionMode m_actionMode = AM_Move;
	float m_gravity = -50.0f;
};