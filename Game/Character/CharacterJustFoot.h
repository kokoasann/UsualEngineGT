#pragma once
//#include "CharacterConst.h"

class Character;
/// <summary>
/// JustFootとは?:  歩いてから止まった時に大抵の場合足の位置がずれるので、
/// それをまるで意思があるかのように調整してくれる機能。
/// (これは余談だが、実は英語でぴったりを意味する "just fit" と掛けている)
/// </summary>
class CharacterJustFoot :public ue::GameObject
{
public:
	CharacterJustFoot(){}
	~CharacterJustFoot(){}

	/// <summary>
	/// 開放。
	/// </summary>
	void Release() override;
	/// <summary>
	/// DeleteGOされた瞬間呼ばれる
	/// </summary>
	void OnDestroy() override;

	/// <summary>
	/// JustFootの初期化。
	/// </summary>
	/// <param name="chara">キャラクター</param>
	/// <param name="offsetY">足の上げる角度の調整。キャラの方向ベクトルのyに足す。足した後にノーマライズされる playerの場合2.0f</param>
	/// <param name="scale">足をどれだけあげるか playerの場合40.0f</param>
	/// <param name="upSpeed">足をあげるスピード playerの場合0.4f</param>
	/// <param name="downSpeed">足をおろすスピード playerの場合0.5f</param>
	void Init(Character* chara,float offsetY, float scale, float upSpeed, float downSpeed)
	{
		m_chara = chara;
		m_justFoot_OffsetY = offsetY;
		m_justFoot_Scale = scale;
		m_justFoot_UpIKSpeed = upSpeed;
		m_justFoot_DownIKSpeed = downSpeed;
	}
	/// <summary>
	/// ボーンの初期化。
	/// </summary>
	/// <param name="footL">左</param>
	/// <param name="footR">右</param>
	void InitBone(ue::Bone* footL, ue::Bone* footR)
	{
		m_footL = footL;
		m_footR = footR;
	}
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

private:
	/// <summary>
	/// 更新本体。
	/// </summary>
	/// <param name="delTime">デルタタイム</param>
	void Update_JustFoot(float delTime);

public:
	/// <summary>
	///	この関数を呼ぶことでJustFootが開始される
	/// </summary>
	void Start_JustFoot();

	bool IsStart_JustFoot() const
	{
		return m_isStart;
	}
private:
	Character* m_chara = nullptr;			//キャラクター

	float m_time = 0.f;						//時間。
	bool m_isStart = false;					//始まる。
	
	ue::Bone* m_footL = nullptr;			//左足のボーン
	ue::Bone* m_footR = nullptr;			//右足のボーン
	bool m_isJustedEnd = false;				//justFootのEnd足が終わったか
	bool m_isJustedStart = false;			//justFootのstart足が終わったか
	//float m_justTime = 0.f;
	enum EJustFoot
	{
		JF_footL,		//左足から調整する。
		JF_footR		//右足から調整する。
	};
	EJustFoot m_startJustFoot = JF_footL;		//どちらの足が先か
	bool m_isStartJustFoot = false;				//歩きからアイドルになって初めてJustFootが呼ばれるときにだけ作動するifを制御するためのフラグ
	float m_justFoot_OffsetY = 2.f;				//足の上げる角度の調整。
	float m_justFoot_Scale = 40.f;				//足をどれだけ上げるか。
	float m_justFoot_UpIKSpeed = 0.4f;			//足を上げるときの速度。
	float m_justFoot_DownIKSpeed = 0.5f;		//足をおろすときの速度。

	ue::CVector3 m_nowUP = ue::CVector3::Zero();							//今、どれだけ上がっているか。
	bool m_isUped = false;
};