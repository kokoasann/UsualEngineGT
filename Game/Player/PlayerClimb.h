#pragma once

class Character;
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
		virtual void Update(const PlayerClimb& body, ue::IK* ik) = 0;
		
		bool isComplete = false;
	};
	/// <summary>
	/// 移動しているときのステート
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
	/// 止まっているときのステート
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
	/// マネージャー
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
	/// 登るときのスペック
	/// </summary>
	struct ClimbSpec
	{
		float upLen = 0.0f;			//移動するときに動かす手足の最大の移動距離。
		float sideLen = 0.0f;		//モデルの中心から横にどれだけ手足を広げるか
		float forwardLen = 0.0f;	//手足をどれだけ前に出すか
		float speed = 0.0f;			//手足を動かすスピード
		ue::CVector2 startHandPos = ue::CVector2::Zero();	//モデルのポジションからの相対的な手の初期位置。左手側のポジションだけで大丈夫。
		ue::CVector2 startFootPos = ue::CVector2::Zero();	//モデルのポジションからの相対的な足の初期位置。左足側のポジションだけで大丈夫。
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
	Character* m_chara = nullptr;		//キャラクター
	ue::IK* m_footLIK = nullptr;		//左足のIK
	ue::IK* m_footRIK = nullptr;		//右足のIK
	ue::IK* m_handLIK = nullptr;		//左手のIK
	ue::IK* m_handRIK = nullptr;		//右手のIK

	LimbState* m_footLState = nullptr;	//左足のステート
	LimbState* m_footRState = nullptr;	//右足のステート
	LimbState* m_handLState = nullptr;	//左手のステート
	LimbState* m_handRState = nullptr;	//右手のステート

	ue::CVector2 m_moveDir = ue::CVector2::Zero();	//動く方向。

	bool m_isClimb = false;							//

	ClimbSpec m_climbSpec;							//登るスペック

	ue::SkinModelRender::MoveFunc m_moveFunc;		//
	ue::SkinModelRender::RotateFunc m_rotateFunc;	//
};