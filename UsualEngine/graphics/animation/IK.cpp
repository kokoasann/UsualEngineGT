#include "PreCompile.h"
#include "IK.h"


namespace UsualEngine
{
	//IK用の当たり判定のやつ
	struct SweepResultIK : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit = false;
		CVector3 hitPos = CVector3::Zero();
		CVector3 startPos = CVector3::Zero();
		CVector3 hitNormal = CVector3::Zero();
		float dist = FLT_MAX;
		btCollisionObject* me = nullptr;
		int chit = 0;

		btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me || convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character)
			{
				chit += 1;
				return 0.0f;
			}
			isHit = true;
			CVector3 hitp = *(CVector3*)& convexResult.m_hitPointLocal;
			CVector3 div = startPos - hitp;
			float dis = div.Length();
			if (dis < dist)
			{
				hitNormal = *(CVector3*)& convexResult.m_hitNormalLocal;
				hitPos = *(CVector3*)& convexResult.m_hitPointLocal;
				dist = dis;
			}
			return 0.0f;
		}
	};

	CMatrix GetBoneWorldMatrix(Bone* bone,const CMatrix& worldMat)
	{
		Bone* parents[32] = { NULL };
		int cont = 0;
		Bone* b = bone;
		while (true)
		{
			parents[cont++] = b;
			b = b->GetParent();
			if (b == nullptr)
				break;
		}
		auto mat = worldMat;
		for (int i = cont - 1; i >= 0; i--)
		{
			auto locmat = parents[i]->GetLocalMatrix();
			mat.Mul(locmat, mat);
		}
		//mat.Mul(bone->GetLocalMatrix(),mat);
		return mat;
	}

	CMatrix GetBoneLocalMatrix(Bone * bone, const CMatrix& wm, const CMatrix& worldMat)
	{
		CMatrix mat = wm;
		CMatrix inv;
		inv.Inverse(GetBoneWorldMatrix(bone->GetParent(),worldMat));
		mat.Mul(wm, inv);
		return mat;
	}


	IK::IK(Bone* effector, Bone* end, float radius, const CVector3& pos) :
		m_effectorBone(effector),
		m_endBone(end),
		m_radius(radius)
	{
		m_collider.Create(radius);
		InitRigidBody(pos);
	}

	IK::~IK()
	{
		
	}

	void IK::InitRigidBody(const CVector3& pos)
	{
		RigidBodyInfo rbinfo;
		rbinfo.mass = 0.f;
		rbinfo.collider = &m_collider;
		m_rigidBody.Create(rbinfo);
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
		m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		auto& rpos = m_rigidBody.GetBody()->getWorldTransform();
		CMatrix base,tr;
		base = CMatrix::Identity();
		tr.MakeTranslation(pos);
		tr.Mul(base, tr);
		auto m = GetBoneWorldMatrix(m_effectorBone, tr);
		rpos.setOrigin(btVector3(m.m[3][0], m.m[3][1], m.m[3][2]));
		Physics().AddRigidBody(m_rigidBody);
	}

	void IK::UpdateTarget(const CMatrix& worldMat)
	{
		auto effmat = GetBoneWorldMatrix(m_effectorBone, worldMat);
		auto effpos = effmat.GetTranslation();
		auto currentBone = m_effectorBone->GetParent();						//作業中のボーン

		auto newpos = effpos;	//移動先のポジション

		auto oldpos = m_effectorBone->GetWorldMatrix().GetTranslation();	//移動前のポジション
		static bool b = 0;
		if (!b)
		{
			oldpos += worldMat.GetTranslation();
			b = 1;
		}
		if ((newpos - oldpos).Length() < 0.000001f)
			return;

		oldpos.y += m_radius;

		SweepResultIK sr;
		sr.startPos = oldpos;
		sr.me = m_rigidBody.GetBody();

		btTransform bstart, bend;
		bstart.setIdentity();
		bend.setIdentity();

		bstart.setOrigin(btVector3(oldpos.x, oldpos.y, oldpos.z));
		bend.setOrigin(btVector3(newpos.x, newpos.y, newpos.z));

		CVector3 target = newpos;//ターゲット
		Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
		if (sr.isHit)
		{
			if (!m_isHit)
			{
				m_effectorBone->SetIsONGround(true);

				auto norm = sr.hitNormal;
				norm.Normalize();
				auto meri = newpos - sr.hitPos;
				float rad = meri.Dot(norm);
				target = sr.hitPos;
				target += sr.hitNormal * (-rad + m_radius + m_radius);
			}
			else
			{
				target.Lerp(m_rubbing,newpos, m_target);
				
			}
		}
		else
		{
			m_effectorBone->SetIsONGround(false);
		}
		m_target = target;
		m_isHit = sr.isHit;
		m_move = newpos-m_target;
		//auto o2n = m_target - oldpos;
		m_effectorBone->SetMove(m_move);

		UpdateRigidBody(m_target);
	}

	void IK::Update(const CMatrix& worldMat)
	{
		switch (m_usingIK)
		{
		case enUSE_CCD:
			EXE_CCD(worldMat);
			break;
		}
	}
	void IK::EXE_CCD(const CMatrix& worldMat)
	{	
#if 0
		auto effmat = GetBoneWorldMatrix(m_effectorBone, worldMat);
		auto effpos = effmat.GetTranslation();
		auto currentBone = m_effectorBone->GetParent();						//作業中のボーン

		auto newpos = effpos;	//移動先のポジション

		//auto localmat = bones[ind]->GetLocalMatrix();
		//auto worldmat = CMatrix::Identity();
		//worldmat.Mul(m_worldMatrix,localmat);
		//auto newpos = worldmat.GetTranslation();	//移動先のポジション

		auto oldpos = m_effectorBone->GetWorldMatrix().GetTranslation();	//移動前のポジション
		if ((newpos - oldpos).Length() < 0.000001f)
			return;

		SweepResultIK sr;
		sr.startPos = oldpos;

		btTransform bstart, bend;
		bstart.setIdentity();
		bend.setIdentity();

		bstart.setOrigin(btVector3(oldpos.x, oldpos.y, oldpos.z));
		bend.setOrigin(btVector3(newpos.x, newpos.y, newpos.z));

		auto target = newpos;			//ターゲット
					//ターゲット
		Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
		if (sr.isHit)
		{
			auto norm = sr.hitNormal;
			norm.Normalize();
			auto meri = newpos - sr.hitPos;
			float rad = meri.Dot(norm);
			target = sr.hitPos;
			target += sr.hitNormal * (-rad + m_radius-5);
			
			auto invworldmat = CMatrix::Identity();
			invworldmat.Inverse(worldMat);

			if (m_effectorBone->IsONGround())
			{
				//target = oldpos;
			}
			m_effectorBone->SetIsONGround(true);

			static int roop = 1;
			for (int i = 0; i < roop; i++)
			{
				while (true)
				{
					auto currentmat = GetBoneWorldMatrix(currentBone, worldMat);//作業ボーンの世界行列。

					auto currentlocal = currentBone->GetLocalMatrix();//作業ボーンのローカル行列。
					CVector3 lopo, losc;
					CQuaternion loro;
					currentlocal.CalcMatrixDecompose(lopo, loro, losc);

					auto currentpos = currentmat.GetTranslation();	//作業中のボーンの位置。

					CMatrix inv;
					inv.Inverse(currentmat);

					CVector3 localeffpos = effpos;
					inv.Mul(localeffpos);
					CVector3 localtarpos = target;
					inv.Mul(localtarpos);

					auto toEffector = localeffpos;			//エフェクターからカレントボーンのベクトル
					auto toTarget = localtarpos;			//ターゲットからカレントボーンのベクトル

					auto elen = toEffector.Length();
					auto tlen = toTarget.Length();

					toEffector.Normalize();
					toTarget.Normalize();

					auto rad = min(1, toEffector.Dot(toTarget));			//二つのベクトルの角度(ラッドウィンプス)
					rad = acos(rad);
					float deg = CMath::RadToDeg(rad);

					if (rad > 0.000001f)
					{
						auto axis = CVector3::Zero();					//回転軸.
						axis.Cross(toEffector, toTarget);
						axis.Normalize();

						auto addrot = CQuaternion::Identity();			//加える回転.
						addrot.SetRotation(axis, rad);
						auto difrot = CQuaternion::Identity();			//
						difrot.SetRotationDeg(axis, 360.f - CMath::RadToDeg(rad));

						CMatrix mRot;									//加える回転行列。
						mRot.MakeRotationFromQuaternion(addrot);


						auto bfloro = loro;
						loro.Multiply(addrot);
						CMatrix msca, mrot, mpos, mfin;



						msca.MakeScaling(losc);
						mrot.MakeRotationFromQuaternion(loro);
						mpos.MakeTranslation(lopo);

						mfin.Mul(msca, mrot);
						mfin.Mul(mfin, mpos);
						currentBone->SetLocalMatrix(mfin);

						//auto fepos = GetBoneWorldMatrix(m_effectorBone, worldMat).GetTranslation();
						//auto fe2target = (fepos - target).Length();
						//auto ef2target = (effpos - target).Length();

						//CMatrix mfin2;
						//bfloro.Multiply(difrot);
						//mrot.MakeRotationFromQuaternion(bfloro);
						//mfin2.Mul(msca, mrot);
						//mfin2.Mul(mfin2, mpos);
						////currentBone->SetLocalMatrix(mfin2);

						//fepos = GetBoneWorldMatrix(m_effectorBone, worldMat).GetTranslation();
						//auto fe2target2 = (fepos - target).Length();
						//if (fe2target < fe2target2)
						//{
						//	//currentBone->SetLocalMatrix(mfin);
						//}
						//else
						//{
						//	int i = 0;
						//}


						/*auto pos = currentlocal.GetTranslation();
						currentlocal.m[3][0] = 0.f;
						currentlocal.m[3][1] = 0.f;
						currentlocal.m[3][2] = 0.f;
						currentlocal.Mul(currentlocal, mRot);
						CMatrix tra;
						tra.MakeTranslation(pos);
						currentlocal.Mul(currentlocal, tra);

						currentBone->SetLocalMatrix(currentlocal);*/
					}


					if (m_endBone == currentBone)
						break;
					effmat = GetBoneWorldMatrix(m_effectorBone, worldMat);
					effpos = effmat.GetTranslation();

					currentBone = currentBone->GetParent();
				}
			}
		}
		else
		{
			m_effectorBone->SetIsONGround(false);
		}
		//break;//debug用。
#else
		if (!m_isHit and false)
			return;
		for (int i = 0; i < 1; i++)
		{
			auto effmat = GetBoneWorldMatrix(m_effectorBone, worldMat);
			auto effpos = effmat.GetTranslation();
			auto currentBone = m_effectorBone->GetParent();						//作業中のボーン
			while (true)
			{
				auto currentmat = GetBoneWorldMatrix(currentBone, worldMat);//作業ボーンの世界行列。

				auto currentlocal = currentBone->GetLocalMatrix();//作業ボーンのローカル行列。
				CVector3 lopo, losc;
				CQuaternion loro;
				currentlocal.CalcMatrixDecompose(lopo, loro, losc);

				auto currentpos = currentmat.GetTranslation();	//作業中のボーンの位置。

				CMatrix inv;							//カレントボーンの逆行列。
				inv.Inverse(currentmat);

				CVector3 localeffpos = effpos;
				inv.Mul(localeffpos);					//ワールド座標のエフェクタボーンをカレントボーンのローカル座標にする
				CVector3 localtarpos = m_target;
				inv.Mul(localtarpos);					//ワールド座標のターゲットをカレントボーンのローカル座標にする

				auto toEffector = localeffpos;			//エフェクターからカレントボーンのベクトル
				auto toTarget = localtarpos;			//ターゲットからカレントボーンのベクトル

				auto elen = toEffector.Length();
				auto tlen = toTarget.Length();

				toEffector.Normalize();
				toTarget.Normalize();

				auto rad = min(1, toEffector.Dot(toTarget));			//二つのベクトルの角度(ラッドウィンプス)
				rad = acos(rad);
				float deg = CMath::RadToDeg(rad);

				if (true || rad > 0.000001f)
				{
					auto axis = CVector3::Zero();					//回転軸.
					axis.Cross(toEffector, toTarget);
					axis.Normalize();

					auto addrot = CQuaternion::Identity();			//加える回転.
					addrot.SetRotation(axis, rad);
					auto difrot = CQuaternion::Identity();			//
					difrot.SetRotationDeg(axis, 360.f - CMath::RadToDeg(rad));

					CMatrix mRot;									//加える回転行列。
					mRot.MakeRotationFromQuaternion(addrot);


					auto bfloro = loro;
					loro.Multiply(addrot);
					CMatrix msca, mrot, mpos, mfin;

					msca.MakeScaling(losc);
					mrot.MakeRotationFromQuaternion(loro);
					mpos.MakeTranslation(lopo);

					mfin.Mul(msca, mrot);
					mfin.Mul(mfin, mpos);
					currentBone->SetLocalMatrix(mfin);
				}


				if (m_endBone == currentBone)
					break;
				effmat = GetBoneWorldMatrix(m_effectorBone, worldMat);
				effpos = effmat.GetTranslation();

				currentBone = currentBone->GetParent();
			}
		}
#endif
		//UpdateRigidBody(m_target);
	}

	void IK::UpdateRigidBody(const CVector3& pos)
	{
		auto body = m_rigidBody.GetBody();
		body->setActivationState(DISABLE_DEACTIVATION);
		auto& bpo = body->getWorldTransform();
		bpo.setOrigin({ pos.x,pos.y, pos.z });
	}
}