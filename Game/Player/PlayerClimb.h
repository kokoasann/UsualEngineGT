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
	/// ステート
	/// 
	/// この構造体を継承した構造体は
	/// newには NewLS
	/// deleteには DelLS
	/// を使ってね
	/// </summary>
	struct LimbState
	{
		/*
		※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
		※※													※※
		※※　LimbStateを継承した構造体にはこれ等を必ず書く		※※
		※※　LimbState_Tempの部分は継承したその構造体の名前	※※
		※※													※※
		※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※


		static std::vector<LimbState_Temp*>TrashBox;				//ゴミ箱　DelLSしたインスタンスが入る場所。この中の物はNewLSで使いまわされる。
		static std::vector<LimbState_Temp*>AllInstance;				//NewLSで生成されたインスタンスが入る場所。
		void Init() override;										//初期化。NewLSで生成(もしくは使いまわし)された時に呼ばれる純粋仮想関数。ちゃんと書かないとエラー起きるぞ。
		/// <summary>
		/// 更新。
		/// </summary>
		/// <param name="body">こいつを保持しているインスタンス</param>
		/// <param name="ik">こいつで動かすIK</param>
		/// <returns>次のStepのenumを返す。進まない場合はStep_Noneを返す</returns>
		unsigned char Update(const PlayerClimb& body, ue::IK* ik) override;	//更新。使い方はご自由に。純粋仮想関数。
		*/

		LimbState()
		{
		}
		virtual ~LimbState(){}
		virtual void Init() = 0;
		/// <summary>
		/// 更新。
		/// </summary>
		/// <param name="body">こいつを保持しているインスタンス</param>
		/// <param name="ik">こいつで動かすIK</param>
		/// <returns>次のStepのenumを返す。進まない場合はStep_Noneを返す</returns>
		virtual LimbStep Update(const PlayerClimb& body, ue::IK* ik) = 0;
	};

	/// <summary>
	/// 手(足)を上げる
	/// </summary>
	struct LimbState_Up :public LimbState
	{
		static std::vector<LimbState_Up*>TrashBox;				//ゴミ箱　DelLSしたインスタンスが入る場所。この中の物はNewLSで使いまわされる。
		static std::vector<LimbState_Up*>AllInstance;				//NewLSで生成されたインスタンスが入る場所。
		void Init() override;										//初期化。NewLSで生成(もしくは使いまわし)された時に呼ばれる純粋仮想関数。ちゃんと書かないとエラー起きるぞ。
		/// <summary>
		/// 更新。
		/// </summary>
		/// <param name="body">こいつを保持しているインスタンス</param>
		/// <param name="ik">こいつで動かすIK</param>
		/// <returns>次のStepのenumを返す。進まない場合はStep_Noneを返す</returns>
		LimbStep Update(const PlayerClimb& body, ue::IK* ik) override;	//更新。使い方はご自由に。純粋仮想関数。
	private:
		float m_upLen = 0.0f;
	};

	/// <summary>
	/// 手(足)を下す
	/// </summary>
	struct LimbState_Down :public LimbState
	{
		static std::vector<LimbState_Down*>TrashBox;				//ゴミ箱　DelLSしたインスタンスが入る場所。この中の物はNewLSで使いまわされる。
		static std::vector<LimbState_Down*>AllInstance;				//NewLSで生成されたインスタンスが入る場所。
		void Init() override;										//初期化。NewLSで生成(もしくは使いまわし)された時に呼ばれる純粋仮想関数。ちゃんと書かないとエラー起きるぞ。
		/// <summary>
		/// 更新。
		/// </summary>
		/// <param name="body">こいつを保持しているインスタンス</param>
		/// <param name="ik">こいつで動かすIK</param>
		/// <returns>次のStepのenumを返す。進まない場合はStep_Noneを返す</returns>
		LimbStep Update(const PlayerClimb& body, ue::IK* ik) override;	//更新。使い方はご自由に。純粋仮想関数。
	private:
		float m_downLen = 0.0f;
	};

	/// <summary>
	/// 移動しているときのステート
	/// </summary>
	struct LimbState_Move:public LimbState
	{
		static std::vector<LimbState_Move*>TrashBox;		//ステートに必須
		static std::vector<LimbState_Move*>AllInstance;		//ステートに必須
		void Init() override;								//ステートに必須
		
		/// <summary>
		/// 更新。
		/// </summary>
		/// <param name="body">こいつを保持しているインスタンス</param>
		/// <param name="ik">こいつで動かすIK</param>
		/// <returns>次のStepのenumを返す。進まない場合はStep_Noneを返す</returns>
		LimbStep Update(const PlayerClimb& body, ue::IK* ik) override;	//ステートに必須
		float timer = 0.0f;

	private:
	};
	/// <summary>
	/// 止まっているときのステート
	/// </summary>
	struct LimbState_Stop :public LimbState
	{
		static std::vector<LimbState_Stop*>TrashBox;
		static std::vector<LimbState_Stop*>AllInstance;
		void Init() override;
		/// <summary>
		/// 更新。
		/// </summary>
		/// <param name="body">こいつを保持しているインスタンス</param>
		/// <param name="ik">こいつで動かすIK</param>
		/// <returns>次のStepのenumを返す。進まない場合はStep_Noneを返す</returns>
		LimbStep Update(const PlayerClimb& body, ue::IK* ik) override;
	private:
	};

	std::vector<LimbState*> TrashBox;
	std::vector<LimbState*> AllInstance;



	/// <summary>
	/// マネージャー　主に(今のところ)各LimbStateの開放を行う
	/// 
	/// 新しいステートを追加したらここのデストラクタに追記しておこう
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

	static LimbStateManager s_limbStateManager;	//マネージャーのインスタンス　アプリ終了時に一気にリリースされるぞ。



	/// <summary>
	/// LimbStateのnew
	/// </summary>
	//template<class LimbState_Temp>
	//inline static LimbState_Temp* NewLS()
	//{
	//	LimbState_Temp* ls = nullptr;
	//	if (LimbState_Temp::TrashBox.size() == 0)//ゴミ箱が空っぽなので新しくインスタンスを生成。
	//	{
	//		ls = new LimbState_Temp();
	//		LimbState_Temp::AllInstance.push_back(ls);
	//	}		
	//	else//ゴミ箱にインスタンスが残っているので使いまわす
	//	{
	//		ls = LimbState_Temp::TrashBox[LimbState_Temp::TrashBox.size() - 1];
	//		LimbState_Temp::TrashBox.pop_back();
	//	}
	//	ls->Init();	//初期化。
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
	/// LimbStateのdelete
	/// 引数に入れたポインタはnullptrになって帰ってきます。
	/// </summary>
	//template<class LimbState_Temp>
	//inline static void DelLS(LimbState_Temp*& ls)
	//{
	//	//ゴミ箱にポイ
	//	LimbState_Temp::TrashBox.push_back(ls);
	//	ls = nullptr;
	//}

	void DelLS(LimbState*& ls)
	{
		TrashBox.push_back(ls);
		ls = nullptr;
	}

	/// <summary>
	/// LimbStateのRelease
	/// 指定したLimbStateの開放を行う
	/// </summary>
	template<class LimbState_Temp>
	inline static void ReleaseLS()
	{
		//今まで作られたインスタンスをすべて破棄。
		for (auto item : LimbState_Temp::AllInstance)
		{
			delete item;
		}
		LimbState_Temp::AllInstance.clear();	//クリア!
		LimbState_Temp::TrashBox.clear();		//クリア!
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
	/// こっから本体。
	/// 
	/// </summary>

	/// <summary>
	/// 登るときのスペック
	/// </summary>
	struct ClimbSpec
	{
		float upLen = 0.0f;			//移動するときに動かす手足の最大の移動距離。
		float sideLen = 0.0f;		//モデルの中心から横にどれだけ手足を広げるか
		float forwardLen = 0.0f;	//手足をどれだけ前に出すか
		float speed = 0.0f;			//手足を動かすスピード
		float changeLug = 0.0f;		//登る方の手(足)を入れ替えるときにかかる時間(秒)
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

private:
	void UpdateLimbState(ue::IK* ik, LimbState*& limbState);
	void UpdateLimb(Limb& limb);
	void UpdatePairLimbs(PairLimbs& pair);
public:
	
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

	PairLimbs m_handLimbs;
	PairLimbs m_footLimbs;

	ue::CVector2 m_moveDir = ue::CVector2::Zero();	//動く方向。
	float m_moveLen = 0.0f;

	bool m_isClimb = false;							//

	ClimbSpec m_climbSpec;							//登るスペック

	ue::SkinModelRender::MoveFunc m_moveFunc;		//
	ue::SkinModelRender::RotateFunc m_rotateFunc;	//
};