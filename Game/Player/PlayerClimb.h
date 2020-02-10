#pragma once


class PlayerClimb:public ue::GameObject
{
private:
	/// <summary>
	/// ステート
	/// </summary>
	struct LimbState
	{
		LimbState()
		{
		}
		virtual ~LimbState(){}
		void Relese(){}
		virtual void Update(ue::IK* ik) = 0;
		
		bool isComplete = false;
	};
	/// <summary>
	/// 移動しているときのステート
	/// </summary>
	struct LimbState_Move:public LimbState
	{
		static std::vector<LimbState_Move*>TrashBox;
		static std::vector<LimbState_Move*>AllInstance;
		static void* operator new(size_t size)
		{
			void* ptr = nullptr;
			if (TrashBox.size() == 0)
			{
				ptr = malloc(size);
				AllInstance.push_back((LimbState_Move*)ptr);
				
			}
			else
			{
				ptr = (void*)TrashBox[TrashBox.size() - 1];
				TrashBox.pop_back();
			}
			return ptr;
		}
		static void operator delete(void* ptr)
		{
			TrashBox.push_back((LimbState_Move*)ptr);
		}
		static void Release()
		{
			for (auto item : AllInstance)
			{
				free(item);
			}
			AllInstance.clear();
			TrashBox.clear();
		}


		~LimbState_Move();
		void Update(ue::IK* ik) override;
		float timer = 0.0f;

	private:
		bool isInTrash = false;
	};
	/// <summary>
	/// 止まっているときのステート
	/// </summary>
	struct LimbState_Stop :public LimbState
	{

		void Update(ue::IK* ik) override;

	private:
		bool isInTrush = false;
	};


public:
	/// <summary>
	/// 登るときのスペック
	/// </summary>
	struct ClimbSpec
	{
		float upLen = 0.0f;			//移動するときに動かす手足の最大の移動距離。
		float sideLen = 0.0f;		//モデルの中心から横にどれだけ手足を広げるか
		float forwardLen = 0.0f;	//手足をどれだけ前に出すか
		float speed = 0.0f;			//手足を動かすスピード
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
	void Init(ue::IK* footL, ue::IK* footR, ue::IK* handL, ue::IK* handR);

	void Update();
	void PostUpdate();

	/// <summary>
	/// 進む方向を更新。
	/// </summary>
	/// <param name="dir">
	/// プレイヤーに相対的な方向でなければならない。
	/// 基本的にZは使わない。
	/// 正規化はこちらで行いますので悪しからず。
	/// </param>
	void UpdateInput(const ue::CVector2& dir);

	/// <summary>
	/// 登り始める
	/// </summary>
	void StartClimb();
	/// <summary>
	/// 登るのをやめる
	/// </summary>
	void BreakClimb();
	/// <summary>
	/// 一旦休憩。
	/// </summary>
	void StopClimb();
private:
	ue::IK* m_footLIK = nullptr;		//左足のIK
	ue::IK* m_footRIK = nullptr;		//右足のIK
	ue::IK* m_handLIK = nullptr;		//左手のIK
	ue::IK* m_handRIK = nullptr;		//右手のIK

	LimbState* m_footLState = nullptr;	//左足のステート
	LimbState* m_footRState = nullptr;	//右足のステート
	LimbState* m_handLState = nullptr;	//左手のステート
	LimbState* m_handRState = nullptr;	//右手のステート

	ue::CVector2 m_moveDir = ue::CVector2::Zero();	//動く方向。

	bool m_isClimb = false;

	ClimbSpec m_climbSpec;							//登るスペック

	ue::SkinModelRender::MoveFunc m_moveFunc;		//
	ue::SkinModelRender::RotateFunc m_rotateFunc;	//
};