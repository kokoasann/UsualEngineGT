/*!
 * @brief	静的オブジェクト。
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/RigidBody.h"

namespace UsualEngine
{
	/*!
	* @brief	静的物理オブジェクト
	*/
	class PhysicsStaticObject {
	public:
		/*!
			* @brief	コンストラクタ。
			*/
		PhysicsStaticObject();
		/*!
			* @brief	デストラクタ。
			*/
		~PhysicsStaticObject();
		/*!
			* @brief	メッシュの静的オブジェクトを作成。
			*@param[in]	skinModel	スキンモデル。
			*@param[in]	pos			座標。
			*@param[in]	rot			回転。
			*/
		void CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot,CVector3 sca);

		MeshCollider& GetMeshCollider()
		{
			return m_meshCollider;
		}
		RigidBody& GetRigidBody()
		{
			return m_rigidBody;
		}
	private:
		MeshCollider m_meshCollider;		//メッシュコライダー。
		RigidBody m_rigidBody;				//剛体。
		bool m_isCreated = false;			//Createは呼ばれた？
	};
}