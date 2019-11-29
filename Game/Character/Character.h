#pragma once

/// <summary>
/// ���f���̃C���X�^���X�̏����A
/// IK�ɂ��ړ��̃T�|�[�g�������Ă���N���X
/// ��{�I�ɃL�����N�^�[�ɂ͂��̃N���X��������
/// </summary>
class Character:public ue::GameObject
{
public:
	/// <summary>
	/// �����̃X�e�[�g
	/// </summary>
	enum ActionMode
	{
		AM_None,
		AM_Move,
		AM_Rotate,
	};

	/// <summary>
	/// �{�[���̕���.
	/// </summary>
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

	//const wchar*
	using wstr = const wchar_t*;

	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="smr"></param>
	/// <param name="ccradius"></param>
	/// <param name="ccheight"></param>
	/// <param name="offset"></param>
	void Init(ue::SkinModelRender* smr,float ccradius=0,float ccheight=0,const ue::CVector3& offset=ue::CVector3::Zero());
	/// <summary>
	/// �A�j���[�V�����̏������B(�����\��)
	/// </summary>
	/// <param name="animclip"></param>
	void InitAnimData(ue::AnimationClip* animclip);
	/// <summary>
	/// �{�[���̏������B(�����\��)
	/// </summary>
	/// <param name="footLname"></param>
	/// <param name="footRname"></param>
	/// <param name="handLname"></param>
	/// <param name="handRname"></param>
	/// <param name="waistLname"></param>
	/// <param name="waistRname"></param>
	void InitBoneData
	(
		wstr footLname = nullptr,
		wstr footRname = nullptr,
		wstr handLname = nullptr,
		wstr handRname = nullptr,
		wstr waistLname = nullptr,
		wstr waistRname = nullptr
	);

	/// <summary>
	/// �{�[����T��
	/// </summary>
	/// <param name="name">�{�[���̖��O</param>
	/// <param name="bk">�{�[���̎��</param>
	/// <param name="isSetingIK">IK������H</param>
	/// <param name="len">IK����͈�</param>
	/// <param name="radius">IK����Ƃ��̃R���C�_�[�̑傫��</param>
	/// <returns></returns>
	ue::Bone* FindBone(wstr name,BoneKind bk=BK_None,bool isSetingIK=false,int len=0,float radius=10.f);
	/// <summary>
	/// �{�[�����Z�b�g����B
	/// </summary>
	/// <param name="bone">�Z�b�g����{�[��</param>
	/// <param name="bk">�ǂ��̕��ʂ��B���Ă͂܂�enum���Ȃ��ꍇ��BK_None</param>
	/// <param name="isSetingIK">IK������H</param>
	/// <param name="len">IK����͈�</param>
	/// <param name="radius">IK����Ƃ��̃R���C�_�[�̑傫��</param>
	void SetBone(ue::Bone* bone, BoneKind bk = BK_None,bool isSetingIK = false, int len = 0, float radius = 10.f);

	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();

	/// <summary>
	/// IK���g�����ړ����邽�߂̓����֐�������
	/// </summary>
	/// <param name="movefunc"></param>
	void SetMoveFunc(const ue::SkinModelRender::MoveFunc& movefunc)
	{
		m_model->SetMoveFunc(movefunc);
	}
	/// <summary>
	/// IK���g������]���邽�߂̓����֐�������
	/// </summary>
	/// <param name="rotatefunc"></param>
	void SetRotateFunc(const ue::SkinModelRender::RotateFunc& rotatefunc)
	{
		m_model->SetRotateFunc(rotatefunc);
	}
	/// <summary>
	/// MoveFunc���f�t�H���g�̂��̂ɂ���
	/// </summary>
	void SetDefaultMoveFunc()
	{
		m_model->SetMoveFunc(m_defaultMoveFunc);
	}
	/// <summary>
	/// RotateFunc���f�t�H���g�̂��̂ɂ���
	/// </summary>
	void SetDefaultRotateFunc()
	{
		m_model->SetRotateFunc(m_defaultRotateFunc);
	}

	/// <summary>
	/// �C�x���g���X�i�[��������
	/// </summary>
	/// <param name="ev"></param>
	void AddEventListener(const ue::Animation::EventListener& ev);

	/// <summary>
	/// ActionMode��Move�ɂ���
	/// </summary>
	void ModeMove();
	/// <summary>
	/// ActionMobe����]�ɂ���
	/// </summary>
	void ModeRotate();

	/// <summary>
	/// �|�W�V������Ԃ�
	/// </summary>
	/// <returns></returns>
	const ue::CVector3& GetPos() const
	{
		return m_model->GetPos();
	}
	/// <summary>
	/// ��]��Ԃ�
	/// </summary>
	/// <returns></returns>
	const ue::CQuaternion& GetRot()
	{
		return m_model->GetRot();
	}

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	/// <param name="anim">�Đ�����A�j���[�V����</param>
	/// <param name="lerp">�A�j���[�V�����̕ۊǎ���(�b) �����l��1�b</param>
	/// <param name="start">�A�j���[�V�����̎n�܂鎞��</param>
	/// <param name="am">�A�j���[�V�����ɂ��A�N�V�����ŉ������������� �ݒ肵�Ȃ��ꍇ�͉������Ȃ�</param>
	void PlayAnim(int anim, float lerp = 1.0f,float start = 0.0f, ActionMode am = AM_None);
	
	/// <summary>
	/// �S��IK�́A���C����f�ɂȂ�B
	/// </summary>
	/// <param name="f">1~0���I�k�k���B���͈̔͊O���w�肷��ƁA�����ςɂȂ�B�^����m�肽���΂���Ă݂邪�����B</param>
	void SetAllIKRub(float f)
	{
		m_model->SetAllIKRub(f);
	}
	/// <summary>
	/// ����move�̕������ړ�����
	/// </summary>
	/// <param name="move"></param>
	void SetMove(const ue::CVector3& move)
	{
		m_move = move;
	}
	/// <summary>
	/// ����rot�̕�������]����
	/// </summary>
	/// <param name="rot"></param>
	void SetRotate(const ue::CQuaternion& rot)
	{
		m_rotate = rot;
	}
	/// <summary>
	/// ��]��
	///	�@���ڂ��I
	/// SkinModelRender�ɓ����
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(const ue::CQuaternion& rot)
	{
		m_model->SetRot(rot);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="bone"></param>
	const ue::CVector3& GetIKTarget(ue::Bone* bone) const
	{
		return m_model->GetAnimation().GetIKTarget(bone);
	}
	/// <summary>
	/// IK�̃^�[�Q�b�g�𓮂����I�t�Z�b�g
	/// </summary>
	/// <param name="v"></param>
	void SetIKOffset(const ue::CVector3& v, ue::Bone* bone = nullptr)
	{
		m_model->GetAnimation().SetIKOffset(v,bone);
	}
	/// <summary>
	/// IK�̃^�[�Q�b�g�𓮂������x(%)1�Ȃ�1�t���[���ňړ�����B0�Ȃ瓮���Ȃ��B
	/// </summary>
	/// <param name="speed"></param>
	/// <param name="bone">speed������{�[��.nullptr�̏ꍇ�͑S�Ă�speed���K�������</param>
	void SetIKSpeed(float speed, ue::Bone* bone = nullptr)
	{
		m_model->GetAnimation().SetIKSpeed(speed, bone);
	}
	/// <summary>
	/// SkinModelRender�̏�������Animation�������Ă���
	/// </summary>
	/// <returns></returns>
	const ue::Animation& GetAnimation() const
	{
		return m_model->GetAnimation();
	}
private:
	ue::SkinModelRender* m_model = nullptr;		//���f���̃|�C���^
	ue::Bone* m_footL = nullptr;				//�����̃{�[��
	ue::Bone* m_footR = nullptr;				//�E���̃{�[��
	ue::Bone* m_handL = nullptr;				//����̃{�[��
	ue::Bone* m_handR = nullptr;				//�E��̃{�[��
	ue::Bone* m_waistL = nullptr;				//�����̃{�[��
	ue::Bone* m_waistR = nullptr;				//�E���̃{�[��
	std::vector<ue::Bone*> m_boneList;			//�{�[���̃��X�g
	ue::CharacterController m_characon;			//�L�����R��
	ue::CVector3 m_ccOffset;					//�L�����R���̃I�t�Z�b�g(��{�I��y������)
	ActionMode m_actionMode = AM_Move;			//�Ȃ�̓��������Ă��邩
	float m_gravity = -200.0f;					//�d�́B
	ue::CVector3 m_momentum = ue::CVector3::Zero();		//����(��������)
	ue::CVector3 m_move = ue::CVector3::Zero();					//�N���X�O����A�N�Z�X���邽�߂̈ړ��x�N�g��
	ue::CQuaternion m_rotate = ue::CQuaternion::Identity();		//�N���X�O����A�N�Z�X���邽�߂̉�]�N�H�[�^�j�I��
	ue::SkinModelRender::MoveFunc m_defaultMoveFunc;			//�f�t�H���g��MoveFunc
	ue::SkinModelRender::RotateFunc m_defaultRotateFunc;		//�f�t�H���g��RotateFunc
};