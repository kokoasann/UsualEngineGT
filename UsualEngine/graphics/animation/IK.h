#pragma once

#include "physics/SphereCollider.h"
#include "Physics/RigidBody.h"

namespace UsualEngine
{
	class IK
	{
	public:
		/// <summary>
		/// IKの種類。
		/// </summary>
		enum UseIK
		{
			enUSE_CCD,
		};

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="effector">エフェクタボーン</param>
		/// <param name="end">エンドボーン(エフェクタボーンの親をたどっていくとエンドボーンになるような関係でなければならない。)</param>
		/// <param name="radius">IKに使用するスフィアコライダーの半径</param>
		IK(Bone* effector, Bone* end, float radius);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~IK();

		/// <summary>
		/// 更新。
		/// </summary>
		/// <param name="worldMat"></param>
		void Update(CMatrix& worldMat);

	private:
		/// <summary>
		/// CCDIKの実行
		/// </summary>
		/// <param name="worldMat"></param>
		void EXE_CCD(CMatrix& worldMat);

		/// <summary>
		/// リジッドボディの初期化。
		/// </summary>
		void InitRigidBody();

		/// <summary>
		/// リジッドボディの更新
		/// </summary>
		/// <param name="pos"></param>
		void UpdateRigidBody(CVector3 pos);

	public:
		/// <summary>
		/// IKの先端のボーン
		/// </summary>
		/// <param name="bone"></param>
		void SetEffectorBone(Bone* bone)
		{
			m_effectorBone = bone;
		}
		/// <summary>
		/// IKの終わりのボーン
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
		int m_usingIK = enUSE_CCD;		//使うIKの種類。

		SphereCollider m_collider;			//コライダー
		RigidBody m_rigidBody;				//リジッドボディ
	};

}