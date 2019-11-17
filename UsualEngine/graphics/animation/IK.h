#pragma once

#include "physics/SphereCollider.h"
#include "Physics/RigidBody.h"

namespace UsualEngine
{
	class IK
	{
	public:
		/// <summary>
		/// 使用するIKの種類。
		/// </summary>
		enum UseIK
		{
			enUSE_CCD,
		};
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="effector">エフェクタボーン</param>
		/// <param name="end">エンドボーン</param>
		/// <param name="radius">コライダーの半径</param>
		IK(Bone* effector, Bone* end, float radius,const CVector3& pos = CVector3::Zero());
		/// <summary>
		/// デストラクタ
		/// </summary>
		~IK();

		/// <summary>
		/// IKのターゲットとなる場所を更新する
		/// </summary>
		/// <param name="worldMat">モデルのワールドマトリクス</param>
		void UpdateTarget(const CMatrix& worldMat);
		/// <summary>
		/// IKの更新。
		/// </summary>
		/// <param name="worldMat">モデルのワールドマトリクス</param>
		void Update(const CMatrix& worldMat);

		/// <summary>
		/// CCD_IKを実行。
		/// </summary>
		/// <param name="worldMat">モデルのワールドマトリクス</param>
		void EXE_CCD(const CMatrix& worldMat);

		/// <summary>
		/// リジッドボディの初期化。
		/// </summary>
		void InitRigidBody(const CVector3& pos);
		/// <summary>
		/// リジッドボディの更新
		/// </summary>
		/// <param name="pos"></param>
		void UpdateRigidBody(const CVector3& pos);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="bone"></param>
		void SetEffectorBone(Bone* bone)
		{
			m_effectorBone = bone;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="bone"></param>
		void SetEndBone(Bone* bone)
		{
			m_endBone = bone;
		}
	private:
		Bone* m_effectorBone=0;			//エフェクタボーン
		Bone* m_endBone=0;					//エンドボーン
		float m_radius = 0.f;					//コライダーの半径。
		int m_usingIK = enUSE_CCD;		//使用するIKの種類。

		SphereCollider m_collider;			//コライダー
		RigidBody m_rigidBody;				//リジッドボディ

		CVector3 m_target = CVector3::Zero();	//IKのターゲット
		CVector3 m_move = CVector3::Zero();	//移動ベクトル
		bool m_isHit = false;			//当たった？
	};

}