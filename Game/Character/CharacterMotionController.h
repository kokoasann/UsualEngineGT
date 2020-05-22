#pragma once
#include "CharacterMoveMotion.h"
#include "CharacterRotateMotion.h"

class Character;

class CharacterMotionController:public ue::GameObject
{
public:
	CharacterMotionController() {};
	virtual ~CharacterMotionController() {};

	/// <summary>
	/// �J���B
	/// </summary>
	virtual void Release() override;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual void OnDestroy() override;

	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="chara"></param>
	/// <param name="animClip"></param>
	void Init(Character* chara,ue::AnimationClip* animClip);
	/// <summary>
	/// �ړ��p�̏������B
	/// </summary>
	/// <param name="footL"></param>
	/// <param name="footR"></param>
	void InitMove(ue::Bone* footL, ue::Bone* footR,float startUPMax = 0.0f, float upSpeed=0.0f);
	/// <summary>
	/// ��]�p�̏������B
	/// </summary>
	/// <param name="rotateMode"></param>
	/// <param name="speedDeg"></param>
	void InitRotate(CharacterRotateMotion::RotateMode rotateMode, float speedDeg);

	//�X�^�[�g
	bool Start();
	//�X�V�B
	void Update();

	/// <summary>
	/// �ړ�����B
	/// </summary>
	/// <param name="playNum"></param>
	/// <param name="speed"></param>
	/// <param name="actionMode"></param>
	void PlayMove(unsigned int playNum,float speed,ActionMode actionMode);
	/// <summary>
	/// ��]����B
	/// </summary>
	/// <param name="dir"></param>
	/// <param name="playNum"></param>
	/// <param name="isPlayAnim"></param>
	void PlayRotate(const ue::CVector3& dir, unsigned int playNum,bool isPlayAnim=false);

	void PlayIdol();
private:
	Character* m_chara = nullptr;			//�L�����N�^�[

	CharacterMoveMotion m_moveMotion;		//�ړ��B
	CharacterRotateMotion m_rotateMotion;	//��]�B

	enum PlayMotion
	{
		PM_Idol,
		PM_Move,
		PM_Rotate,
	};
	PlayMotion m_playMotion = PM_Idol;

	unsigned int m_idolNum = 0;

	int m_initCount = 0;					//����������?
	bool m_isPlatAnimRotate = false;		//��]���ɃA�j���[�V�������Đ����Ă���?

	
};