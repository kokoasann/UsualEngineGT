#pragma once

class MortarShell:public ue::GameObject
{
public:
	MortarShell(){}
	~MortarShell();
	/// <summary>
	/// 開放。
	/// </summary>
	void Release() override{}
	/// <summary>
	/// DeleteGOされた瞬間呼ばれる
	/// </summary>
	void OnDestroy() override;
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="pos">大砲のポジション</param>
	/// <param name="dir">向いている方向</param>
	void Init(ue::CVector3 pos,ue::CVector3 dir);
	/// <summary>
	/// Start
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

private:
	ue::SkinModelRender* m_model = nullptr;						//モデル
	ue::CVector3 m_pos = ue::CVector3::Zero();					//ポジション
	ue::CVector3 m_dir = ue::CVector3::Zero();						//方向と力を兼ね備えたベクトル
	ue::CVector3 m_oldDir = ue::CVector3::Zero();				//古い向き(使わない(多分))
	ue::CQuaternion m_rot = ue::CQuaternion::Identity();		//回転。

	float m_power = 8000.f;// / 50;										//ちから
	float m_gravity = 9.8f * 350;// / 50;									//重力。
	float m_time = 0.0f;															//時間。(使わない(多分))
	ue::SphereCollider m_collider;											//コライダー
};