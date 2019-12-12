#pragma once
#include "CharacterConst.h"

class Character;
class CharacterRotateMotion :public ue::GameObject
{
public:
	using PlayAnim = unsigned int;
	enum RotateMode
	{
		RM_None,	//回転しない
		RM_Direct,	//即座に指定された方向を向く
		RM_Lerp,	//線形補間しながら指定された方向を向く
		RM_UseIK	//IKを使用した回転を利用して指定された方向を向く
	};

	CharacterRotateMotion(){}
	~CharacterRotateMotion(){}

	/// <summary>
	/// 開放。
	/// </summary>
	void Release() override{}
	/// <summary>
	/// DeleteGOされた瞬間呼ばれる
	/// </summary>
	void OnDestroy() override{}


	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="chara">キャラクター</param>
	/// <param name="rm">回転の仕方</param>
	/// <param name="speed">回転の速度Deg</param>
	void Init(Character* chara,RotateMode rm,float speedDeg)
	{
		m_chara = chara;
		m_rotateMode = rm;
		m_speed = ue::CMath::DegToRad(speedDeg);
	}

	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

private:
	/// <summary>
	/// ベクトルから直接回転させるときの更新処理。
	/// </summary>
	void Update_Direct();
	/// <summary>
	/// 現在向いている方向から入力した方向ベクトルに線形補間しながら回転させるときの更新処理。
	/// </summary>
	void Update_Lerp();
	/// <summary>
	/// IKを使った回転をさせるときの更新処理。
	/// </summary>
	void Update_UseIK();

	/// <summary>
	/// 回転が完了したときに呼ばれる関数。
	/// </summary>
	void Complete();

public:
	/// <summary>
	/// 回転の仕方を設定する
	/// </summary>
	/// <param name="rm"></param>
	void SetRotateMode(RotateMode rm)
	{
		m_rotateMode = rm;
	}
	/// <summary>
	/// 回転スピードを設定する(ラディアン)
	/// </summary>
	/// <param name="rad">ラディアン</param>
	void SetSpeed(float rad)
	{
		m_speed = rad;
	}
	/// <summary>
	/// 回転スピードを設定する(デグリー)
	/// </summary>
	/// <param name="deg">デグリー</param>
	void SetSpeedDeg(float deg)
	{
		m_speed = ue::CMath::DegToRad(deg);
	}

	/// <summary>
	/// dirの方に向かせる
	/// </summary>
	/// <param name="dir">dirはその回転させるキャラからの相対的なものでなくてはならない</param>
	/// <param name="pa"></param>
	/// <param name="isPlayAnim"></param>
	void NextRotate(const ue::CVector3& dir, PlayAnim pa, bool isPlayAnim = false);
	
private:
	Character* m_chara = nullptr;					//キャラクター

	ActionMode m_actionMode = AM_None;				//アクションモード(いらないかも)
	RotateMode m_rotateMode = RM_None;				//ローテートモード
	PlayAnim m_playingAnim = 0;						//再生しているアニメ

	bool m_isPlayAnim = false;						//アニメを再生する
	bool m_isFirst = true;							//NextRotate関数を読んでから初めて更新されるときに反応するフラグ
	bool m_isComplete = false;						//回転完了。
	float m_total = 0.f;							//回転量(lerpの時にしか使わない)
	float m_maxRad = 0.f;							//最大回転量(lerpの時にしか使わない)
	float m_speed = ue::CMath::DegToRad(5.f);		//回転の速度。
	float m_threshold = 0.0001f;					//閾値。
	ue::CVector3 m_dir = ue::CVector3::AxisZ();		//回転先の方向。
	ue::CVector3 m_oldDir = m_dir;					//今現在の方向。
};