#pragma once
#include "CharacterConst.h"
class Character;
class CharacterMoveMotion :public ue::GameObject
{
public:
	using PlayAnim = unsigned int;	//再生するアニメーションの番号。intじゃわかりずらいので別名定義。
	CharacterMoveMotion(){}
	virtual ~CharacterMoveMotion() {}

	/// <summary>
	/// 開放。
	/// </summary>
	void Release() override;
	/// <summary>
	/// DeleteGOされた瞬間呼ばれる
	/// </summary>
	void OnDestroy() override;

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="chara">Characterのポインタ</param>
	/// <param name="ac">アニメーションクリップの配列</param>
	void Init(Character* chara, ue::AnimationClip* ac)
	{
		m_chara = chara;
		m_anim = ac;
	}
	/// <summary>
	/// 足ボーンの初期化。
	/// </summary>
	/// <param name="footL">左</param>
	/// <param name="footR">右</param>
	void InitBone(ue::Bone* footL, ue::Bone* footR);

	void InitStartUpFoot(float upMax,float upspeed);
	
	/// <summary>
	/// JustFootの初期化。
	/// JustFootとは?:  歩いてから止まった時に大抵の場合足の位置がずれるので、
	/// それをまるで意思があるかのように調整してくれる機能。(実は英語でぴったりを意味する "just fit" と掛けている)
	/// </summary>
	/// <param name="offsetY">足の上げる角度の調整。キャラの方向ベクトルのyに足す。足した後にノーマライズされる playerの場合2.0f</param>
	/// <param name="scale">足をどれだけあげるか playerの場合40.0f</param>
	/// <param name="upSpeed">足をあげるスピード playerの場合0.4f</param>
	/// <param name="downSpeed">足をおろすスピード playerの場合0.5f</param>
	void InitJustFoot(float offsetY, float scale, float upSpeed, float downSpeed)
	{

		/*m_justFoot_OffsetY = offsetY;
		m_justFoot_Scale = scale;
		m_justFoot_UpIKSpeed = upSpeed;
		m_justFoot_DownIKSpeed = downSpeed;*/
	}
	//更新
	void Update()override;

private:
	/// <summary>
	/// 実際に動く
	/// </summary>
	/// <param name="delTime">デルタタイム</param>
	/// <param name="pa">次に再生するアニメーション</param>
	/// <param name="movespeed"></param>
	/// <param name="am"></param>
	void Move(float delTime, PlayAnim pa, float movespeed, ActionMode am);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="delTime">デルタタイム</param>
	void Walk2Idle(float delTime);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="delTime">デルタタイム</param>
	void JustFoot(float delTime);

	/// <summary>
	/// アニメーションが変わった時に呼ばれる関数。
	/// </summary>
	/// <param name="pa">再生するアニメ</param>
	void ChangePlayingAnim(PlayAnim pa);

public:
	/// <summary>
	/// 次に再生するアニメーション
	/// </summary>
	/// <param name="pa">アニメーションクリップの配列の番号</param>
	/// <param name="speed">動く速度</param>
	/// <param name="am">
	/// IKを利用した移動をしたい場合はAM_Move,
	/// 移動したいけどIKを利用した移動をしたくない場合はAM_None,
	/// それ以外の場合は移動しない
	/// </param>
	void NextPlayAnim(PlayAnim pa,float speed, ActionMode am)
	{
		m_doPlayAnim = pa;
		m_speed = speed;
		m_actionMode = am;
	}

	void BreakAnim();
private:
	Character* m_chara = nullptr;					//外身。
	ue::AnimationClip* m_anim = nullptr;		//アニメーションクリップ

	ActionMode m_actionMode = AM_None;				//IKでの移動をさせたくない場合はAM_None　詳細はActionModeを参照。
	PlayAnim m_doPlayAnim = 0;									//次に再生するアニメーション

	float m_time = 0.f;							//時間。汎用タイマー
	bool m_isWalk = false;						//歩いているかそうでないか

	PlayAnim m_playingAnim = 0;		//今再生しているアニメーション
	PlayAnim m_oldPlayAnim = 0;		//前に再生していたアニメーション
	PlayAnim m_idolNum = 0;				//アニメーションのidolの番号。
	float m_animLugBase = 0.6f;			//アイドルから歩きに変わるときにかかる時間。
	//float m_animLug = 0.2f;
	float m_animLug_2idle = 0.0f;			//歩きからアイドルに移り変わる時間。
	//float m_lugTime = 0.f;
	ue::CVector3 m_moved = ue::CVector3::AxisZ();		//動いていたベクトル。
	//ue::CVector3 m_dir = ue::CVector3::AxisZ();
	float m_speed = 0.f;							//動くスピード
	float m_oldSpeedBuff = 0.0f;			//古いスピードのバッファ
	float m_oldSpeed = 0.0f;					//古いスピード
	enum EChangeWalk
	{
		CW_slow2fast,		//遅い移動から速い移動に変化した
		CW_fast2slow		//速い移動から遅い移動に変化した
	};
	EChangeWalk m_changeWalk = CW_slow2fast;		//動きの変化の仕方。

	ue::Bone* m_footL = nullptr;			//左足のボーン
	ue::Bone* m_footR = nullptr;			//右足のボーン
	ue::IK* m_footLIK = nullptr;
	bool m_isJustFoot = false;							//justFootを行えるか？

	bool m_isUpfoot = false;
	bool m_isStartUpFoot = false;
	float m_startUpMax = 0.0f;
	float m_startUpSpeed = 0.f;
	float m_startUpNow = 0.0f;
};