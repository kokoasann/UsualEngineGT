#pragma once

class Character;
class PlayerClimb:public ue::GameObject
{
private:
	
	/// <summary>
	/// �X�e�[�g
	/// </summary>
	struct LimbState
	{
		LimbState()
		{
		}
		virtual ~LimbState(){}
		virtual void Update(const PlayerClimb& body, ue::IK* ik) = 0;
		
		bool isComplete = false;
	};
	/// <summary>
	/// �ړ����Ă���Ƃ��̃X�e�[�g
	/// </summary>
	struct LimbState_Move:public LimbState
	{
		static std::vector<LimbState_Move*>TrashBox;
		static std::vector<LimbState_Move*>AllInstance;
		void* operator new(size_t size);
		
		void operator delete(void* ptr)
		{
			TrashBox.push_back((LimbState_Move*)ptr);
		}
		static void Release();
		
		~LimbState_Move();
		void Update(const PlayerClimb& body, ue::IK* ik) override;
		float timer = 0.0f;

	private:
		bool isInTrash = false;
	};
	/// <summary>
	/// �~�܂��Ă���Ƃ��̃X�e�[�g
	/// </summary>
	struct LimbState_Stop :public LimbState
	{
		void Update(const PlayerClimb& body, ue::IK* ik) override;
	private:
		bool isInTrush = false;
	};

	template<class LimbState_Temp>
	LimbState_Temp* NewLS()
	{
		LimbState_Temp* ls = nullptr;
		if (TrashBox.size() == 0)
		{
			ls = new LimbState_Temp();
			LimbState_Temp::AllInstance.push_back(ls);
		}		else
		{
			ls = LimbState_Temp::TrashBox[LimbState_Temp::TrashBox.size() - 1];
			LimbState_Temp::TrashBox.pop_back();
		}
		return ls;
	}
	template<class LimbState_Temp>
	void DelLS(LimbState_Temp* ls)
	{
		LimbState_Temp::TrashBox.push_back(ls)
	}

	/// <summary>
	/// �}�l�[�W���[
	/// </summary>
	struct LimbStateManager
	{
		~LimbStateManager()
		{
			LimbState_Move::Release();
		}
	};
	
	static LimbStateManager s_limbStateManager;

public:
	/// <summary>
	/// �o��Ƃ��̃X�y�b�N
	/// </summary>
	struct ClimbSpec
	{
		float upLen = 0.0f;			//�ړ�����Ƃ��ɓ������葫�̍ő�̈ړ������B
		float sideLen = 0.0f;		//���f���̒��S���牡�ɂǂꂾ���葫���L���邩
		float forwardLen = 0.0f;	//�葫���ǂꂾ���O�ɏo����
		float speed = 0.0f;			//�葫�𓮂����X�s�[�h
		ue::CVector2 startHandPos = ue::CVector2::Zero();	//���f���̃|�W�V��������̑��ΓI�Ȏ�̏����ʒu�B���葤�̃|�W�V���������ő��v�B
		ue::CVector2 startFootPos = ue::CVector2::Zero();	//���f���̃|�W�V��������̑��ΓI�ȑ��̏����ʒu�B�������̃|�W�V���������ő��v�B
	};

	PlayerClimb();
	virtual ~PlayerClimb();

	virtual void Release() override;
	virtual void OnDestroy() override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="footL"></param>
	/// <param name="footR"></param>
	/// <param name="handL"></param>
	/// <param name="handR"></param>
	void Init(Character* chara,ue::IK* footL, ue::IK* footR, ue::IK* handL, ue::IK* handR);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="spec"></param>
	void InitClimbSpec(const ClimbSpec& spec);

	void Update();
	void PostUpdate();

	/// <summary>
	/// �i�ޕ������X�V�B
	/// </summary>
	/// <param name="dir">
	/// �v���C���[�ɑ��ΓI�ȕ����łȂ���΂Ȃ�Ȃ��B
	/// ��{�I��Z�͎g��Ȃ��B
	/// ���K���͂�����ōs���܂��̂ň������炸�B
	/// </param>
	void UpdateInput(const ue::CVector2& dir);

	/// <summary>
	/// �o��n�߂�
	/// </summary>
	void StartClimb();
	/// <summary>
	/// �o��̂���߂�
	/// </summary>
	void BreakClimb();
	/// <summary>
	/// ��U�x�e�B
	/// </summary>
	void StopClimb();
private:
	Character* m_chara = nullptr;		//�L�����N�^�[
	ue::IK* m_footLIK = nullptr;		//������IK
	ue::IK* m_footRIK = nullptr;		//�E����IK
	ue::IK* m_handLIK = nullptr;		//�����IK
	ue::IK* m_handRIK = nullptr;		//�E���IK

	LimbState* m_footLState = nullptr;	//�����̃X�e�[�g
	LimbState* m_footRState = nullptr;	//�E���̃X�e�[�g
	LimbState* m_handLState = nullptr;	//����̃X�e�[�g
	LimbState* m_handRState = nullptr;	//�E��̃X�e�[�g

	ue::CVector2 m_moveDir = ue::CVector2::Zero();	//���������B

	bool m_isClimb = false;							//

	ClimbSpec m_climbSpec;							//�o��X�y�b�N

	ue::SkinModelRender::MoveFunc m_moveFunc;		//
	ue::SkinModelRender::RotateFunc m_rotateFunc;	//
};