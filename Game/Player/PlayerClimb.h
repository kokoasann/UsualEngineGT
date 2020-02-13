#pragma once

class Character;
class PlayerClimb:public ue::GameObject
{
private:
	enum LimbStep
	{
		Step_None,
		Step_UP,
		Step_Move,
		Step_Down,
		Step_Stop,
	};
	/// <summary>
	/// �X�e�[�g
	/// 
	/// ���̍\���̂��p�������\���̂�
	/// new�ɂ� NewLS
	/// delete�ɂ� DelLS
	/// ���g���Ă�
	/// </summary>
	struct LimbState
	{
		/*
		������������������������������������������������������������
		����													����
		�����@LimbState���p�������\���̂ɂ͂��ꓙ��K������		����
		�����@LimbState_Temp�̕����͌p���������̍\���̖̂��O	����
		����													����
		������������������������������������������������������������


		static std::vector<LimbState_Temp*>TrashBox;				//�S�~���@DelLS�����C���X�^���X������ꏊ�B���̒��̕���NewLS�Ŏg���܂킳���B
		static std::vector<LimbState_Temp*>AllInstance;				//NewLS�Ő������ꂽ�C���X�^���X������ꏊ�B
		void Init() override;										//�������BNewLS�Ő���(�������͎g���܂킵)���ꂽ���ɌĂ΂�鏃�����z�֐��B�����Ə����Ȃ��ƃG���[�N���邼�B
		/// <summary>
		/// �X�V�B
		/// </summary>
		/// <param name="body">������ێ����Ă���C���X�^���X</param>
		/// <param name="ik">�����œ�����IK</param>
		/// <returns>����Step��enum��Ԃ��B�i�܂Ȃ��ꍇ��Step_None��Ԃ�</returns>
		unsigned char Update(const PlayerClimb& body, ue::IK* ik) override;	//�X�V�B�g�����͂����R�ɁB�������z�֐��B
		*/

		LimbState()
		{
		}
		virtual ~LimbState(){}
		virtual void Init() = 0;
		/// <summary>
		/// �X�V�B
		/// </summary>
		/// <param name="body">������ێ����Ă���C���X�^���X</param>
		/// <param name="ik">�����œ�����IK</param>
		/// <returns>����Step��enum��Ԃ��B�i�܂Ȃ��ꍇ��Step_None��Ԃ�</returns>
		virtual LimbStep Update(const PlayerClimb& body, ue::IK* ik) = 0;
	};

	/// <summary>
	/// ��(��)���グ��
	/// </summary>
	struct LimbState_Up :public LimbState
	{
		static std::vector<LimbState_Up*>TrashBox;				//�S�~���@DelLS�����C���X�^���X������ꏊ�B���̒��̕���NewLS�Ŏg���܂킳���B
		static std::vector<LimbState_Up*>AllInstance;				//NewLS�Ő������ꂽ�C���X�^���X������ꏊ�B
		void Init() override;										//�������BNewLS�Ő���(�������͎g���܂킵)���ꂽ���ɌĂ΂�鏃�����z�֐��B�����Ə����Ȃ��ƃG���[�N���邼�B
		/// <summary>
		/// �X�V�B
		/// </summary>
		/// <param name="body">������ێ����Ă���C���X�^���X</param>
		/// <param name="ik">�����œ�����IK</param>
		/// <returns>����Step��enum��Ԃ��B�i�܂Ȃ��ꍇ��Step_None��Ԃ�</returns>
		LimbStep Update(const PlayerClimb& body, ue::IK* ik) override;	//�X�V�B�g�����͂����R�ɁB�������z�֐��B
	private:
		float m_upLen = 0.0f;
	};

	/// <summary>
	/// ��(��)������
	/// </summary>
	struct LimbState_Down :public LimbState
	{
		static std::vector<LimbState_Down*>TrashBox;				//�S�~���@DelLS�����C���X�^���X������ꏊ�B���̒��̕���NewLS�Ŏg���܂킳���B
		static std::vector<LimbState_Down*>AllInstance;				//NewLS�Ő������ꂽ�C���X�^���X������ꏊ�B
		void Init() override;										//�������BNewLS�Ő���(�������͎g���܂킵)���ꂽ���ɌĂ΂�鏃�����z�֐��B�����Ə����Ȃ��ƃG���[�N���邼�B
		/// <summary>
		/// �X�V�B
		/// </summary>
		/// <param name="body">������ێ����Ă���C���X�^���X</param>
		/// <param name="ik">�����œ�����IK</param>
		/// <returns>����Step��enum��Ԃ��B�i�܂Ȃ��ꍇ��Step_None��Ԃ�</returns>
		LimbStep Update(const PlayerClimb& body, ue::IK* ik) override;	//�X�V�B�g�����͂����R�ɁB�������z�֐��B
	private:
		float m_downLen = 0.0f;
	};

	/// <summary>
	/// �ړ����Ă���Ƃ��̃X�e�[�g
	/// </summary>
	struct LimbState_Move:public LimbState
	{
		static std::vector<LimbState_Move*>TrashBox;		//�X�e�[�g�ɕK�{
		static std::vector<LimbState_Move*>AllInstance;		//�X�e�[�g�ɕK�{
		void Init() override;								//�X�e�[�g�ɕK�{
		
		/// <summary>
		/// �X�V�B
		/// </summary>
		/// <param name="body">������ێ����Ă���C���X�^���X</param>
		/// <param name="ik">�����œ�����IK</param>
		/// <returns>����Step��enum��Ԃ��B�i�܂Ȃ��ꍇ��Step_None��Ԃ�</returns>
		LimbStep Update(const PlayerClimb& body, ue::IK* ik) override;	//�X�e�[�g�ɕK�{
		float timer = 0.0f;

	private:
	};
	/// <summary>
	/// �~�܂��Ă���Ƃ��̃X�e�[�g
	/// </summary>
	struct LimbState_Stop :public LimbState
	{
		static std::vector<LimbState_Stop*>TrashBox;
		static std::vector<LimbState_Stop*>AllInstance;
		void Init() override;
		/// <summary>
		/// �X�V�B
		/// </summary>
		/// <param name="body">������ێ����Ă���C���X�^���X</param>
		/// <param name="ik">�����œ�����IK</param>
		/// <returns>����Step��enum��Ԃ��B�i�܂Ȃ��ꍇ��Step_None��Ԃ�</returns>
		LimbStep Update(const PlayerClimb& body, ue::IK* ik) override;
	private:
	};

	std::vector<LimbState*> TrashBox;
	std::vector<LimbState*> AllInstance;



	/// <summary>
	/// �}�l�[�W���[�@���(���̂Ƃ���)�eLimbState�̊J�����s��
	/// 
	/// �V�����X�e�[�g��ǉ������炱���̃f�X�g���N�^�ɒǋL���Ă�����
	/// 
	/// </summary>
	struct LimbStateManager
	{
		~LimbStateManager()
		{
			ReleaseLS<LimbState_Move>();
			ReleaseLS<LimbState_Stop>();
		}
	};

	static LimbStateManager s_limbStateManager;	//�}�l�[�W���[�̃C���X�^���X�@�A�v���I�����Ɉ�C�Ƀ����[�X����邼�B



	/// <summary>
	/// LimbState��new
	/// </summary>
	//template<class LimbState_Temp>
	//inline static LimbState_Temp* NewLS()
	//{
	//	LimbState_Temp* ls = nullptr;
	//	if (LimbState_Temp::TrashBox.size() == 0)//�S�~��������ۂȂ̂ŐV�����C���X�^���X�𐶐��B
	//	{
	//		ls = new LimbState_Temp();
	//		LimbState_Temp::AllInstance.push_back(ls);
	//	}		
	//	else//�S�~���ɃC���X�^���X���c���Ă���̂Ŏg���܂킷
	//	{
	//		ls = LimbState_Temp::TrashBox[LimbState_Temp::TrashBox.size() - 1];
	//		LimbState_Temp::TrashBox.pop_back();
	//	}
	//	ls->Init();	//�������B
	//	return ls;
	//}
	template<class LimbState_Temp>
	inline LimbState_Temp* NewLS()
	{
		LimbState_Temp* ls = nullptr;
		if (TrashBox.size() == 0)
		{
			ls = new LimbState_Temp();
			AllInstance.push_back(ls);
		}
		else
		{
			for (auto trash : TrashBox)
			{
				if (typeid(&trash) == typeid(LimbState_Temp))
				{
					ls = dynamic_cast<LimbState_Temp>(trash);
				}
			}
			if (ls == nullptr)
			{
				ls = new LimbState_Temp();
				AllInstance.push_back(ls);
			}
		}
		return ls;
	}
	/// <summary>
	/// LimbState��delete
	/// �����ɓ��ꂽ�|�C���^��nullptr�ɂȂ��ċA���Ă��܂��B
	/// </summary>
	//template<class LimbState_Temp>
	//inline static void DelLS(LimbState_Temp*& ls)
	//{
	//	//�S�~���Ƀ|�C
	//	LimbState_Temp::TrashBox.push_back(ls);
	//	ls = nullptr;
	//}

	void DelLS(LimbState*& ls)
	{
		TrashBox.push_back(ls);
		ls = nullptr;
	}

	/// <summary>
	/// LimbState��Release
	/// �w�肵��LimbState�̊J�����s��
	/// </summary>
	template<class LimbState_Temp>
	inline static void ReleaseLS()
	{
		//���܂ō��ꂽ�C���X�^���X�����ׂĔj���B
		for (auto item : LimbState_Temp::AllInstance)
		{
			delete item;
		}
		LimbState_Temp::AllInstance.clear();	//�N���A!
		LimbState_Temp::TrashBox.clear();		//�N���A!
	}

public:
	struct Limb
	{
		Limb() {};
		ue::IK* ik = nullptr;
		LimbState* limbState = nullptr;
		LimbStep nowStep = Step_None;
	};
	struct PairLimbs
	{
		enum LR
		{
			Pair_None,
			Pair_Left,
			Pair_Right,
		};
		Limb leftLimb;
		Limb rightLimb;
		LR moveLimb = Pair_None;
		float timer = 0.0f;
	};

public:
	/// <summary>
	/// 
	/// ��������{�́B
	/// 
	/// </summary>

	/// <summary>
	/// �o��Ƃ��̃X�y�b�N
	/// </summary>
	struct ClimbSpec
	{
		float upLen = 0.0f;			//�ړ�����Ƃ��ɓ������葫�̍ő�̈ړ������B
		float sideLen = 0.0f;		//���f���̒��S���牡�ɂǂꂾ���葫���L���邩
		float forwardLen = 0.0f;	//�葫���ǂꂾ���O�ɏo����
		float speed = 0.0f;			//�葫�𓮂����X�s�[�h
		float changeLug = 0.0f;		//�o����̎�(��)�����ւ���Ƃ��ɂ����鎞��(�b)
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

private:
	void UpdateLimbState(ue::IK* ik, LimbState*& limbState);
	void UpdateLimb(Limb& limb);
	void UpdatePairLimbs(PairLimbs& pair);
public:
	
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

	PairLimbs m_handLimbs;
	PairLimbs m_footLimbs;

	ue::CVector2 m_moveDir = ue::CVector2::Zero();	//���������B
	float m_moveLen = 0.0f;

	bool m_isClimb = false;							//

	ClimbSpec m_climbSpec;							//�o��X�y�b�N

	ue::SkinModelRender::MoveFunc m_moveFunc;		//
	ue::SkinModelRender::RotateFunc m_rotateFunc;	//
};