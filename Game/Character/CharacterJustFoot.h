#pragma once
//#include "CharacterConst.h"

class Character;
/// <summary>
/// JustFoot�Ƃ�?:  �����Ă���~�܂������ɑ��̏ꍇ���̈ʒu�������̂ŁA
/// ������܂�ňӎv�����邩�̂悤�ɒ������Ă����@�\�B
/// (����͗]�k�����A���͉p��ł҂�������Ӗ����� "just fit" �Ɗ|���Ă���)
/// </summary>
class CharacterJustFoot :public ue::GameObject
{
public:
	CharacterJustFoot(){}
	~CharacterJustFoot(){}

	/// <summary>
	/// �J���B
	/// </summary>
	void Release() override;
	/// <summary>
	/// DeleteGO���ꂽ�u�ԌĂ΂��
	/// </summary>
	void OnDestroy() override;

	/// <summary>
	/// JustFoot�̏������B
	/// </summary>
	/// <param name="chara">�L�����N�^�[</param>
	/// <param name="offsetY">���̏グ��p�x�̒����B�L�����̕����x�N�g����y�ɑ����B��������Ƀm�[�}���C�Y����� player�̏ꍇ2.0f</param>
	/// <param name="scale">�����ǂꂾ�������邩 player�̏ꍇ40.0f</param>
	/// <param name="upSpeed">����������X�s�[�h player�̏ꍇ0.4f</param>
	/// <param name="downSpeed">�������낷�X�s�[�h player�̏ꍇ0.5f</param>
	void Init(Character* chara,float offsetY, float scale, float upSpeed, float downSpeed)
	{
		m_chara = chara;
		m_justFoot_OffsetY = offsetY;
		m_justFoot_Scale = scale;
		m_justFoot_UpIKSpeed = upSpeed;
		m_justFoot_DownIKSpeed = downSpeed;
	}
	/// <summary>
	/// �{�[���̏������B
	/// </summary>
	/// <param name="footL">��</param>
	/// <param name="footR">�E</param>
	void InitBone(ue::Bone* footL, ue::Bone* footR)
	{
		m_footL = footL;
		m_footR = footR;
	}
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;

private:
	/// <summary>
	/// �X�V�{�́B
	/// </summary>
	/// <param name="delTime">�f���^�^�C��</param>
	void Update_JustFoot(float delTime);

public:
	/// <summary>
	///	���̊֐����ĂԂ��Ƃ�JustFoot���J�n�����
	/// </summary>
	void Start_JustFoot();

	bool IsStart_JustFoot() const
	{
		return m_isStart;
	}
private:
	Character* m_chara = nullptr;			//�L�����N�^�[

	float m_time = 0.f;						//���ԁB
	bool m_isStart = false;					//�n�܂�B
	
	ue::Bone* m_footL = nullptr;			//�����̃{�[��
	ue::Bone* m_footR = nullptr;			//�E���̃{�[��
	bool m_isJustedEnd = false;				//justFoot��End�����I�������
	bool m_isJustedStart = false;			//justFoot��start�����I�������
	//float m_justTime = 0.f;
	enum EJustFoot
	{
		JF_footL,		//�������璲������B
		JF_footR		//�E�����璲������B
	};
	EJustFoot m_startJustFoot = JF_footL;		//�ǂ���̑����悩
	bool m_isStartJustFoot = false;				//��������A�C�h���ɂȂ��ď��߂�JustFoot���Ă΂��Ƃ��ɂ����쓮����if�𐧌䂷�邽�߂̃t���O
	float m_justFoot_OffsetY = 2.f;				//���̏グ��p�x�̒����B
	float m_justFoot_Scale = 40.f;				//�����ǂꂾ���グ�邩�B
	float m_justFoot_UpIKSpeed = 0.4f;			//�����グ��Ƃ��̑��x�B
	float m_justFoot_DownIKSpeed = 0.5f;		//�������낷�Ƃ��̑��x�B

	ue::CVector3 m_nowUP = ue::CVector3::Zero();							//���A�ǂꂾ���オ���Ă��邩�B
	bool m_isUped = false;
};