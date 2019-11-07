#pragma once


class Character:public ue::GameObject
{
public:
	/// <summary>
	/// 動きのステート
	/// </summary>
	enum ActionMode
	{
		AM_Move,
		AM_Rotate,
	};

	/// <summary>
	/// ボーンの部位.
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
	/// 初期化。
	/// </summary>
	/// <param name="smr"></param>
	/// <param name="ccradius"></param>
	/// <param name="ccheight"></param>
	/// <param name="offset"></param>
	void Init(ue::SkinModelRender* smr,float ccradius,float ccheight,const ue::CVector3& offset=ue::CVector3::Zero());
	/// <summary>
	/// アニメーションの初期化。(消す予定)
	/// </summary>
	/// <param name="animclip"></param>
	void InitAnimData(ue::AnimationClip* animclip);
	/// <summary>
	/// ボーンの初期化。(消す予定)
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
	/// ボーンを探す
	/// </summary>
	/// <param name="name">ボーンの名前</param>
	/// <param name="bk">ボーンの種類</param>
	/// <param name="isSetingIK">IKをする？</param>
	/// <param name="len">IKする範囲</param>
	/// <param name="radius">IKするときのコライダーの大きさ</param>
	/// <returns></returns>
	ue::Bone* FindBone(wstr name,BoneKind bk=BK_None,bool isSetingIK=false,int len=0,float radius=10.f);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// IKを使った移動するための匿名関数を入れる
	/// </summary>
	/// <param name="movefunc"></param>
	void SetMoveFunc(const ue::SkinModelRender::MoveFunc& movefunc);
	/// <summary>
	/// IKを使った回転するための匿名関数を入れる
	/// </summary>
	/// <param name="rotatefunc"></param>
	void SetRotateFunc(const ue::SkinModelRender::RotateFunc& rotatefunc);

	/// <summary>
	/// イベントリスナーを加える
	/// </summary>
	/// <param name="ev"></param>
	void AddEventListener(const ue::Animation::EventListener& ev);

	/// <summary>
	/// ActionModeをMoveにする
	/// </summary>
	void ModeMove();
	/// <summary>
	/// ActionMobeを回転にする
	/// </summary>
	void ModeRotate();

	/// <summary>
	/// ポジションを返す
	/// </summary>
	/// <returns></returns>
	const ue::CVector3& GetPos();
	/// <summary>
	/// 回転を返す
	/// </summary>
	/// <returns></returns>
	const ue::CQuaternion& GetRot();
private:
	ue::SkinModelRender* m_model = nullptr;		//モデルのポインタ
	ue::Bone* m_footL = nullptr;				//左足のボーン
	ue::Bone* m_footR = nullptr;				//右足のボーン
	ue::Bone* m_handL = nullptr;				//左手のボーン
	ue::Bone* m_handR = nullptr;				//右手のボーン
	ue::Bone* m_waistL = nullptr;				//左腰のボーン
	ue::Bone* m_waistR = nullptr;				//右腰のボーン
	std::vector<ue::Bone*> m_boneList;			//ボーンのリスト
	ue::CharacterController m_characon;			//キャラコン
	ue::CVector3 m_ccOffset;					//キャラコンのオフセット(基本的にy軸だけ)
	ue::AnimationClip* m_animClip = nullptr;	//アニメーションクリップ
	ActionMode m_actionMode = AM_Move;			//なんの動きをしているか
	float m_gravity = -50.0f;					//重力。
};