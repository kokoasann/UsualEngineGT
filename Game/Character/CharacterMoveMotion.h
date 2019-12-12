#pragma once
#include "CharacterConst.h"
class Character;
class CharacterMoveMotion :public ue::GameObject
{
public:
	using PlayAnim = unsigned int;	//�Đ�����A�j���[�V�����̔ԍ��Bint����킩�肸�炢�̂ŕʖ���`�B
	CharacterMoveMotion(){}
	~CharacterMoveMotion(){}

	/// <summary>
	/// �J���B
	/// </summary>
	void Release() override{}
	/// <summary>
	/// DeleteGO���ꂽ�u�ԌĂ΂��
	/// </summary>
	void OnDestroy() override{}

	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="chara">Character�̃|�C���^</param>
	/// <param name="ac">�A�j���[�V�����N���b�v�̔z��</param>
	void Init(Character* chara, ue::AnimationClip* ac)
	{
		m_chara = chara;
		m_anim = ac;
	}
	/// <summary>
	/// ���{�[���̏������B
	/// </summary>
	/// <param name="footL">��</param>
	/// <param name="footR">�E</param>
	void InitBone(ue::Bone* footL, ue::Bone* footR)
	{
		m_footL = footL;
		m_footR = footR;
	}
	/// <summary>
	/// JustFoot�̏������B
	/// JustFoot�Ƃ�?:  �����Ă���~�܂������ɑ��̏ꍇ���̈ʒu�������̂ŁA
	/// ������܂�ňӎv�����邩�̂悤�ɒ������Ă����@�\�B(���͉p��ł҂�������Ӗ����� "just fit" �Ɗ|���Ă���)
	/// </summary>
	/// <param name="offsetY">���̏グ��p�x�̒����B�L�����̕����x�N�g����y�ɑ����B��������Ƀm�[�}���C�Y����� player�̏ꍇ2.0f</param>
	/// <param name="scale">�����ǂꂾ�������邩 player�̏ꍇ40.0f</param>
	/// <param name="upSpeed">����������X�s�[�h player�̏ꍇ0.4f</param>
	/// <param name="downSpeed">�������낷�X�s�[�h player�̏ꍇ0.5f</param>
	void InitJustFoot(float offsetY, float scale, float upSpeed, float downSpeed)
	{
		m_justFoot_OffsetY = offsetY;
		m_justFoot_Scale = scale;
		m_justFoot_UpIKSpeed = upSpeed;
		m_justFoot_DownIKSpeed = downSpeed;
	}
	//�X�V
	void Update()override;

private:
	/// <summary>
	/// ���ۂɓ���
	/// </summary>
	/// <param name="delTime">�f���^�^�C��</param>
	/// <param name="pa">���ɍĐ�����A�j���[�V����</param>
	/// <param name="movespeed"></param>
	/// <param name="am"></param>
	void Move(float delTime, PlayAnim pa, float movespeed, ActionMode am);
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

	/// <summary>
	/// �A�j���[�V�������ς�������ɌĂ΂��֐��B
	/// </summary>
	/// <param name="pa">�Đ�����A�j��</param>
	void ChangePlayingAnim(PlayAnim pa);

public:
	/// <summary>
	/// ���ɍĐ�����A�j���[�V����
	/// </summary>
	/// <param name="pa">�A�j���[�V�����N���b�v�̔z��̔ԍ�</param>
	/// <param name="speed">�������x</param>
	/// <param name="am">
	/// IK�𗘗p�����ړ����������ꍇ��AM_Move,
	/// �ړ�����������IK�𗘗p�����ړ����������Ȃ��ꍇ��AM_None,
	/// ����ȊO�̏ꍇ�͈ړ����Ȃ�
	/// </param>
	void NextPlayAnim(PlayAnim pa,float speed, ActionMode am)
	{
		m_doPlayAnim = pa;
		m_speed = speed;
		m_actionMode = am;
	}
private:
	Character* m_chara = nullptr;					//�O�g�B
	ue::AnimationClip* m_anim = nullptr;		//�A�j���[�V�����N���b�v

	ActionMode m_actionMode = AM_None;				//IK�ł̈ړ������������Ȃ��ꍇ��AM_None�@�ڍׂ�ActionMode���Q�ƁB
	PlayAnim m_doPlayAnim = 0;									//���ɍĐ�����A�j���[�V����

	//static ue::SkinModelRender::MoveFunc m_noneMF;		
	//static ue::SkinModelRender::RotateFunc m_noneRF;
	float m_time = 0.f;							//���ԁB�ėp�^�C�}�[
	bool m_isWalk = false;						//�����Ă��邩�����łȂ���

	PlayAnim m_playingAnim = 0;		//���Đ����Ă���A�j���[�V����
	PlayAnim m_oldPlayAnim = 0;		//�O�ɍĐ����Ă����A�j���[�V����
	PlayAnim m_idolNum = 0;				//�A�j���[�V������idol�̔ԍ��B
	float m_animLugBase = 0.6f;			//�A�C�h����������ɕς��Ƃ��ɂ����鎞�ԁB
	//float m_animLug = 0.2f;
	float m_animLug_2idle = 0.0f;			//��������A�C�h���Ɉڂ�ς�鎞�ԁB
	//float m_lugTime = 0.f;
	ue::CVector3 m_moved = ue::CVector3::AxisZ();		//�����Ă����x�N�g���B
	//ue::CVector3 m_dir = ue::CVector3::AxisZ();
	float m_speed = 0.f;							//�����X�s�[�h
	float m_oldSpeedBuff = 0.0f;			//�Â��X�s�[�h�̃o�b�t�@
	float m_oldSpeed = 0.0f;					//�Â��X�s�[�h
	enum EChangeWalk
	{
		CW_slow2fast,		//�x���ړ����瑬���ړ��ɕω�����
		CW_fast2slow		//�����ړ�����x���ړ��ɕω�����
	};
	EChangeWalk m_changeWalk = CW_slow2fast;		//�����̕ω��̎d���B

	ue::Bone* m_footL = nullptr;			//�����̃{�[��
	ue::Bone* m_footR = nullptr;			//�E���̃{�[��
	bool m_isJustedEnd = false;			//justFoot��End�����I�������
	bool m_isJustedStart = false;			//justFoot��start�����I�������
	//float m_justTime = 0.f;
	enum EJustFoot
	{
		JF_footL,		//�������璲������B
		JF_footR		//�E�����璲������B
	};
	EJustFoot m_startJustFoot = JF_footL;			//�ǂ���̑����悩
	bool m_isStartJustFoot = false;					//��������A�C�h���ɂȂ��ď��߂�JustFoot���Ă΂��Ƃ��ɂ����쓮����if�𐧌䂷�邽�߂̃t���O
	float m_justFoot_OffsetY = 2.f;					//���̏グ��p�x�̒����B
	float m_justFoot_Scale = 40.f;						//�����ǂꂾ���グ�邩�B
	float m_justFoot_UpIKSpeed = 0.4f;			//�����グ��Ƃ��̑��x�B
	float m_justFoot_DownIKSpeed = 0.5f;		//�������낷�Ƃ��̑��x�B

	bool m_isJustFoot = false;							//justFoot���s���邩�H
};