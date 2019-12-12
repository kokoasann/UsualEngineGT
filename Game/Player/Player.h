#pragma once
#include "Character/Character.h"
#include "PlayerCamera.h"
#include "PlayerMotion.h"
#include "Physics/SphereCollider.h"

class Cannon;

#define PLAYER_COMPONENT_NUM 3
class Player : public ue::GameObject
{
public:
	enum PlayerAnim
	{
		PA_idol,
		PA_walk,
		PA_walkFast,
		PA_dush,

		PA_num
	};

	Player();
	~Player();

	/// <summary>
	/// 開放。
	/// </summary>
	void Release() override;
	/// <summary>
	/// DeleteGOされた瞬間呼ばれる
	/// </summary>
	void OnDestroy() override;

	bool Start() override;
	void Update() override;

	/// <summary>
	/// プレイヤーのポジションを返す
	/// </summary>
	/// <returns></returns>
	const ue::CVector3& GetPos() const
	{
		return m_chara.GetPos();
	}

private:
	Character m_chara;					//外身。
	PlayerCamera m_camera;				//カメラ
	PlayerMotion m_motion;				//プレイヤーを動かすクラス。
	GameObject* m_gmList[PLAYER_COMPONENT_NUM] = { 0 };	//コンポーネント。上3つのインスタンスのポインタをここに格納する

	ue::AnimationClip m_anim[PA_num];		//アニメーション
	ue::Pad* m_pad = 0;								//ゲームパッド

	float m_HP = 10.f;			//ヒットポイント
	float m_AP = 10.f;			//アクションポイント

	ue::SphereCollider m_collider;
	ue::SkinModel m_cannonMesh;			//キャノンのコリジョンの作成に必要なSkinModel
	std::vector<Cannon*> m_cannons;		//今出してるキャノンの配列。

	bool m_isSettingCannon = false;
};