/*!
 * @brief	�ÓI�I�u�W�F�N�g�B
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
		//���b�V���R���C�_�[���쐬�B
		m_meshCollider.CreateFromSkinModel(skinModel, &mat);
		//���̂��쐬�A
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_meshCollider; //���̂Ɍ`��(�R���C�_�[)��ݒ肷��B
		rbInfo.mass = 0.0f;
		rbInfo.pos = {0,0,0};
		rbInfo.rot = rot;
		//rbInfo.sca = sca;
		m_rigidBody.Create(rbInfo);
		//���̂𕨗����[���h�ɒǉ�����B
		Physics().AddRigidBody(m_rigidBody);
	}
}