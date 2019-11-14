#pragma once

class Player;

class PlayerCamera :public ue::GameObject
{
public:
	//コンストラクタ
	PlayerCamera();
	//デストラクタ
	void OnDestroy() override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="p">プレイヤーのポインタ</param>
	void Init(Player* p);

	//Start
	bool Start();
	//更新
	void Update();

	ue::Camera* GetCamera()
	{
		return m_camera;
	}
private:
	ue::Camera* m_camera = nullptr;					//カメラのポインタ
	ue::CVector3 m_pos = ue::CVector3::Zero();		//position
	ue::CVector3 m_target = ue::CVector3::Zero();	//target
	float m_c2tLen = 0.f;							//カメラのポジションからターゲットの距離。
	float m_p2cLen = 0.f;							//プレイヤーからカメラの距離。
	ue::CVector3 m_p2c = ue::CVector3::Zero();		//プレイヤーからカメラのベクトル。
	ue::CVector3 m_c2t = ue::CVector3::Zero();		//カメラからターゲットのベクトル。
	float m_offsetY = 100.f;						//カメラのターゲットとポジションをY軸にいくつ動かすか。
	Player* m_player = nullptr;						//プレイヤーのポインタ
};