#pragma once
#include "CharacterConst.h"
#include "CharacterJustFoot.h"
/// <summary>
/// モデルのインスタンスの所持、
/// IKによる移動のサポート等をしているクラス
/// 基本的にキャラクターにはこのクラスを持たす
/// </summary>
class Character:public ue::GameObject
{
public:
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
	void Init(ue::SkinModelRender* smr,float ccradius=0,float ccheight=0,const ue::CVector3& offset=ue::CVector3::Zero());

	/// <summary>
	/// JustFootの初期化。
	/// </summary>
	/// <param name="offsetY">足の上げる角度の調整。キャラの方向ベクトルのyに足す。足した後にノーマライズされる playerの場合2.0f</param>
	/// <param name="scale">足をどれだけあげるか playerの場合40.0f</param>
	/// <param name="upSpeed">足をあげるスピード playerの場合0.4f</param>
	/// <param name="downSpeed">足をおろすスピード playerの場合0.5f</param>
	void Init_JustFoot(float offsetY, float scale, float upSpeed, float downSpeed)
	{
		m_justFoot.Init(this,offsetY, scale, upSpeed, downSpeed);
		m_justFoot.InitBone(m_footL, m_footR);
		m_isUseJustFoot = true;
	}

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
	/// ボーンをセットする。
	/// </summary>
	/// <param name="bone">セットするボーン</param>
	/// <param name="bk">どこの部位か。当てはまるenumがない場合はBK_None</param>
	/// <param name="isSetingIK">IKをする？</param>
	/// <param name="len">IKする範囲</param>
	/// <param name="radius">IKするときのコライダーの大きさ</param>
	void SetBone(ue::Bone* bone, BoneKind bk = BK_None,bool isSetingIK = false, int len = 0, float radius = 10.f);

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// IKを使った移動するための匿名関数を入れる
	/// </summary>
	/// <param name="movefunc"></param>
	void SetMoveFunc(const ue::SkinModelRender::MoveFunc& movefunc)
	{
		m_model->SetMoveFunc(movefunc);
	}
	/// <summary>
	/// IKを使った回転するための匿名関数を入れる
	/// </summary>
	/// <param name="rotatefunc"></param>
	void SetRotateFunc(const ue::SkinModelRender::RotateFunc& rotatefunc)
	{
		m_model->SetRotateFunc(rotatefunc);
	}
	/// <summary>
	/// MoveFuncをデフォルトのものにする
	/// </summary>
	void SetDefaultMoveFunc()
	{
		m_model->SetMoveFunc(m_defaultMoveFunc);
	}
	/// <summary>
	/// RotateFuncをデフォルトのものにする
	/// </summary>
	void SetDefaultRotateFunc()
	{
		m_model->SetRotateFunc(m_defaultRotateFunc);
	}

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
	const ue::CVector3& GetPos() const
	{
		return m_model->GetPos();
	}
	/// <summary>
	/// 回転を返す
	/// </summary>
	/// <returns></returns>
	const ue::CQuaternion& GetRot()
	{
		return m_model->GetRot();
	}

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	/// <param name="anim">再生するアニメーション</param>
	/// <param name="lerp">アニメーションの保管時間(秒) 初期値は1秒</param>
	/// <param name="start">アニメーションの始まる時間</param>
	/// <param name="am">アニメーションによるアクションで何をさせたいか 設定しない場合は何もしない</param>
	void PlayAnim(int anim, float lerp = 1.0f,float start = 0.0f, ActionMode am = AM_None);

	/// <summary>
	/// JustFootを開始する。
	/// </summary>
	void Start_JustFoot()
	{
		if (!m_isUseJustFoot || m_justFoot.IsStart_JustFoot())
			return;
		m_justFoot.Start_JustFoot();
	}
	
	/// <summary>
	/// 全てIKは、摩擦率がfになる。
	/// </summary>
	/// <param name="f">1~0がオヌヌメ。この範囲外を指定すると、多分変になる。真実を知りたくばやってみるがいい。</param>
	void SetAllIKRub(float f)
	{
		m_model->SetAllIKRub(f);
	}
	/// <summary>
	/// 引数moveの分だけ移動する
	/// </summary>
	/// <param name="move"></param>
	void SetMove(const ue::CVector3& move)
	{
		m_move = move;
	}
	/// <summary>
	/// 引数rotの分だけ回転する
	/// </summary>
	/// <param name="rot"></param>
	void SetRotate(const ue::CQuaternion& rot)
	{
		m_rotate = rot;
	}
	/// <summary>
	/// 回転を
	///	　直接っ！
	/// SkinModelRenderに入れる
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(const ue::CQuaternion& rot)
	{
		m_model->SetRot(rot);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="am"></param>
	void SetActionMode(ActionMode am)
	{
		m_actionMode = am;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	ActionMode GetActionMode()
	{
		return m_actionMode;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const ue::CVector3& GetDir() const
	{
		return m_dir;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="bone"></param>
	const ue::CVector3& GetIKTarget(ue::Bone* bone) const
	{
		return m_model->GetAnimation().GetIKTarget(bone);
	}
	/// <summary>
	/// IKのターゲットを動かすオフセット
	/// </summary>
	/// <param name="v"></param>
	void SetIKOffset(const ue::CVector3& v, ue::Bone* bone = nullptr)
	{
		m_model->GetAnimation().SetIKOffset(v,bone);
	}
	/// <summary>
	/// IKのターゲットを動かす速度(%)1なら1フレームで移動する。0なら動かない。
	/// </summary>
	/// <param name="speed"></param>
	/// <param name="bone">speedを入れるボーン.nullptrの場合は全てにspeedが適応される</param>
	void SetIKSpeed(float speed, ue::Bone* bone = nullptr)
	{
		m_model->GetAnimation().SetIKSpeed(speed, bone);
	}
	/// <summary>
	/// SkinModelRenderの所持するAnimationを持ってくる
	/// </summary>
	/// <returns></returns>
	const ue::Animation& GetAnimation() const
	{
		return m_model->GetAnimation();
	}
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
	ActionMode m_actionMode = AM_Move;			//なんの動きをしているか
	float m_gravity = -200.0f;					//重力。
	ue::CVector3 m_momentum = ue::CVector3::Zero();		//勢い(多分消す)
	ue::CVector3 m_move = ue::CVector3::Zero();					//クラス外からアクセスするための移動ベクトル
	ue::CQuaternion m_rotate = ue::CQuaternion::Identity();		//クラス外からアクセスするための回転クォータニオン
	ue::CVector3 m_dir = ue::CVector3::AxisZ();								//向いてる方向。
	ue::SkinModelRender::MoveFunc m_defaultMoveFunc;			//デフォルトのMoveFunc
	ue::SkinModelRender::RotateFunc m_defaultRotateFunc;		//デフォルトのRotateFunc

	CharacterJustFoot m_justFoot;				//JustFootSystem
	bool m_isUseJustFoot = false;				//JustFootを使用する？
};