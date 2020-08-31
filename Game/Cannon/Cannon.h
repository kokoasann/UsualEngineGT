#pragma once
#include "physics/MeshCollider.h"

class Cannon :public ue::GameObject
{
public:
	Cannon(){}
	~Cannon() {}
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
	/// <param name="cannonMesh">コライダー用のモデル</param>
	/// <param name="pos">ポジション</param>
	/// <param name="rot">回転</param>
	void Init(const ue::SkinModel& cannonMesh,const ue::CVector3& pos,const ue::CQuaternion& rot);
	/// <summary>
	/// スタート
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// リジッドボディーの更新。
	/// </summary>
	void Update_RigidBody();
	/// <summary>
	/// 発射!!
	/// </summary>
	void Fire();

	/// <summary>
	/// 今からキャノンの編集をします宣言。
	/// </summary>
	void DirEdit()
	{
		m_isDirEdit = true;
	}
	/// <summary>
	/// 編集を終わります宣言。
	/// </summary>
	void EndDirEdit()
	{
		m_isDirEdit = false;
	}

	/// <summary>
	/// キャノンのスケルトンをゲット
	/// </summary>
	/// <returns></returns>
	ue::Skeleton& GetSkeleton()
	{
		return m_cannon->GetSkinModel().GetSkeleton();
	}
	/// <summary>
	/// IKを動かすかどうか
	/// </summary>
	/// <param name="b"></param>
	void SetIKActive(bool b)
	{
		m_cannon->GetAnimation().SetIKActive(b);
	}
	/// <summary>
	/// ポジションを返す
	/// </summary>
	/// <returns></returns>
	const ue::CVector3& GetPos() const
	{
		return m_pos;
	}
	/// <summary>
	/// 回転を返す
	/// </summary>
	/// <returns></returns>
	const ue::CQuaternion& GetRot() const
	{
		return m_rot;
	}
	/// <summary>
	/// 方向をセット
	/// </summary>
	/// <param name="dir"></param>
	void SetDir(const ue::CVector3& dir)
	{
		m_dir = dir;
	}
	/// <summary>
	/// ターゲットをセット
	/// </summary>
	/// <param name="dir"></param>
	void SetTarget(const ue::CVector3& dir)
	{
		auto nonY = dir;
		nonY.y = 0;
		nonY.Normalize();
		float r = atan2(nonY.x, nonY.z);
		ue::CQuaternion rot;
		rot.SetRotation(ue::CVector3::Up(), r);
		m_cannon->SetRot(rot);
		m_stand->SetRot(rot);
		m_rot = rot;
		m_cannon->GetAnimation().SetIKNextTarget(m_dirBone->GetParent()->GetWorldMatrix().GetTranslation()+(dir*8.f), m_dirBone);
		m_dir = dir;
	}
	/// <summary>
	/// 方向用のボーンをゲット
	/// </summary>
	/// <returns></returns>
	ue::Bone* GetDirBone()
	{
		return m_dirBone;
	}
private:
	ue::SkinModelRender* m_cannon = nullptr;		//キャノンのモデル
	ue::SkinModelRender* m_stand = nullptr;			//台のモデル
	ue::MeshCollider m_meshCollider;				//メッシュコライダー
	ue::RigidBody m_rigidBody;						//リジッドボディ

	ue::CVector3 m_dir = ue::CVector3::AxisZ();		//方向。
	ue::CVector3 m_pos = ue::CVector3::Zero();		//ポジション
	ue::CQuaternion m_rot = ue::CQuaternion::Identity();	//回転。
	ue::Bone* m_dirBone = nullptr;							//方向ボーン
	//ue::CQuaternion m_collisionRot = ue::CQuaternion::Identity();	//
	bool m_isDirEdit = false;		//編集中？
	int m_isFirst = 0;			//初めてか？
};