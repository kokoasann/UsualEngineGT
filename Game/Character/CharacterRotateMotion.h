#pragma once
#include "CharacterConst.h"

class Character;
class CharacterRotateMotion :public ue::GameObject
{
public:
	using PlayAnim = unsigned int;
	enum RotateMode
	{
		RM_None,	//��]���Ȃ�
		RM_Direct,	//�����Ɏw�肳�ꂽ����������
		RM_Lerp,	//���`��Ԃ��Ȃ���w�肳�ꂽ����������
		RM_UseIK	//IK���g�p������]�𗘗p���Ďw�肳�ꂽ����������
	};

	CharacterRotateMotion(){}
	~CharacterRotateMotion(){}

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
	/// <param name="chara">�L�����N�^�[</param>
	/// <param name="rm">��]�̎d��</param>
	/// <param name="speed">��]�̑��xDeg</param>
	void Init(Character* chara,RotateMode rm,float speedDeg)
	{
		m_chara = chara;
		m_rotateMode = rm;
		m_speed = ue::CMath::DegToRad(speedDeg);
	}

	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;

private:
	/// <summary>
	/// �x�N�g�����璼�ډ�]������Ƃ��̍X�V�����B
	/// </summary>
	void Update_Direct();
	/// <summary>
	/// ���݌����Ă������������͂��������x�N�g���ɐ��`��Ԃ��Ȃ����]������Ƃ��̍X�V�����B
	/// </summary>
	void Update_Lerp();
	/// <summary>
	/// IK���g������]��������Ƃ��̍X�V�����B
	/// </summary>
	void Update_UseIK();

	/// <summary>
	/// ��]�����������Ƃ��ɌĂ΂��֐��B
	/// </summary>
	void Complete();

public:
	/// <summary>
	/// ��]�̎d����ݒ肷��
	/// </summary>
	/// <param name="rm"></param>
	void SetRotateMode(RotateMode rm)
	{
		m_rotateMode = rm;
	}
	/// <summary>
	/// ��]�X�s�[�h��ݒ肷��(���f�B�A��)
	/// </summary>
	/// <param name="rad">���f�B�A��</param>
	void SetSpeed(float rad)
	{
		m_speed = rad;
	}
	/// <summary>
	/// ��]�X�s�[�h��ݒ肷��(�f�O���[)
	/// </summary>
	/// <param name="deg">�f�O���[</param>
	void SetSpeedDeg(float deg)
	{
		m_speed = ue::CMath::DegToRad(deg);
	}

	/// <summary>
	/// dir�̕��Ɍ�������
	/// </summary>
	/// <param name="dir">dir�͂��̉�]������L��������̑��ΓI�Ȃ��̂łȂ��Ă͂Ȃ�Ȃ�</param>
	/// <param name="pa"></param>
	/// <param name="isPlayAnim"></param>
	void NextRotate(const ue::CVector3& dir, PlayAnim pa, bool isPlayAnim = false);
	
private:
	Character* m_chara = nullptr;					//�L�����N�^�[

	ActionMode m_actionMode = AM_None;				//�A�N�V�������[�h(����Ȃ�����)
	RotateMode m_rotateMode = RM_None;				//���[�e�[�g���[�h
	PlayAnim m_playingAnim = 0;						//�Đ����Ă���A�j��

	bool m_isPlayAnim = false;						//�A�j�����Đ�����
	bool m_isFirst = true;							//NextRotate�֐���ǂ�ł��珉�߂čX�V�����Ƃ��ɔ�������t���O
	bool m_isComplete = false;						//��]�����B
	float m_total = 0.f;							//��]��(lerp�̎��ɂ����g��Ȃ�)
	float m_maxRad = 0.f;							//�ő��]��(lerp�̎��ɂ����g��Ȃ�)
	float m_speed = ue::CMath::DegToRad(5.f);		//��]�̑��x�B
	float m_threshold = 0.0001f;					//臒l�B
	ue::CVector3 m_dir = ue::CVector3::AxisZ();		//��]��̕����B
	ue::CVector3 m_oldDir = m_dir;					//�����݂̕����B
};