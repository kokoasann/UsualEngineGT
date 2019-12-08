/*!
 * @brief	静的オブジェクト。
 */


#include "PreCompile.h"
#include "physics/PhysicsStaticObject.h"

namespace UsualEngine
{
	PhysicsStaticObject::PhysicsStaticObject()
	{
	}
	PhysicsStaticObject::~PhysicsStaticObject()
	{
		Physics().RemoveRigidBody(m_rigidBody);
	}

	void PhysicsStaticObject::CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot, CVector3 sca)
	{
		CMatrix mTra, mRot, mSca;
		mTra.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mSca.MakeScaling(sca);
		CMatrix mat;
		mat.Mul(mSca, mRot);
		mat.Mul(mat, mTra);
		//メッシュコライダーを作成。
		m_meshCollider.CreateFromSkinModel(skinModel, &mat);
		//剛体を作成、
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_meshCollider; //剛体に形状(コライダー)を設定する。
		rbInfo.mass = 0.0f;
		rbInfo.pos = {0,0,0};
		rbInfo.rot = rot;
		//rbInfo.sca = sca;
		m_rigidBody.Create(rbInfo);
		//剛体を物理ワールドに追加する。
		Physics().AddRigidBody(m_rigidBody);
	}
}