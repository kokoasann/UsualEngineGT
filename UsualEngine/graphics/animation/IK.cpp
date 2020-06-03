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
		btCollisionObject* hitCollision = nullptr;

		btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Character
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_NonHitIK)
			{
				chit += 1;
				return 0.0f;
			}
			isHit = true;
			CVector3 hitp = *(CVector3*)&convexResult.m_hitPointLocal;
			CVector3 div = startPos - hitp;
			float dis = div.Length();
			if (dis < dist)
			{
				hitNormal = *(CVector3*)&convexResult.m_hitNormalLocal;
				hitNormal.Normalize();
				hitPos = *(CVector3*)&convexResult.m_hitPointLocal;
				dist = dis;
				hitCollision = (btCollisionObject*)convexResult.m_hitCollisionObject;
			}
			return 0.0f;
		}
	};

	//IK用の当たり判定のやつ
	struct SweepResultIK_Floor : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit = false;
		CVector3 hitPos = CVector3::Zero();
		CVector3 startPos = CVector3::Zero();
		CVector3 hitNormal = CVector3::Zero();
		float dist = FLT_MAX;
		btCollisionObject* me = nullptr;
		int chit = 0;
		btCollisionObject* hitCollision = nullptr;

		btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me 
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Character
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_NonHitIK)
			{
				chit += 1;
				return 0.0f;
			}
			CVector3 norm;
			norm.Set(convexResult.m_hitNormalLocal);
			norm.Normalize();
			auto rad = fabsf(acosf(norm.Dot(CVector3::Up())));
			if (rad < CMath::PI * 0.3f)
			{
				isHit = true;
				CVector3 hitp = *(CVector3*)& convexResult.m_hitPointLocal;
				CVector3 div = startPos - hitp;
				float dis = div.Length();
				if (dis < dist)
				{
					hitNormal = *(CVector3*)& convexResult.m_hitNormalLocal;
					hitNormal.Normalize();
					hitPos = *(CVector3*)& convexResult.m_hitPointLocal;
					dist = dis;

					hitCollision = (btCollisionObject*)convexResult.m_hitCollisionObject;
				}
			}
			return 0.0f;
		}
	};
	//IK用の当たり判定のやつ(壁用)
	struct SweepResultIK_Wall : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit = false;
		CVector3 hitPos = CVector3::Zero();
		CVector3 startPos = CVector3::Zero();
		CVector3 hitNormal = CVector3::Zero();
		float dist = FLT_MAX;
		btCollisionObject* me = nullptr;
		int chit = 0;
		btCollisionObject* hitCollision = nullptr;

		btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Character
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_NonHitIK)
			{
				chit += 1;
				return 0.0f;
			}
			CVector3 norm;
			norm.Set(convexResult.m_hitNormalLocal);
			norm.Normalize();
			auto rad = fabsf(acosf(norm.Dot(CVector3::Up())));
			if (rad >= CMath::PI * 0.3f && rad < CMath::PI * 0.85f)
			{
				isHit = true;
				CVector3 hitp = *(CVector3*)& convexResult.m_hitPointLocal;
				CVector3 div = startPos - hitp;
				float dis = div.Length();
				if (dis < dist)
				{
					hitNormal = *(CVector3*)& convexResult.m_hitNormalLocal;
					hitPos = *(CVector3*)& convexResult.m_hitPointLocal;
					dist = dis;
					hitCollision = (btCollisionObject*)convexResult.m_hitCollisionObject;
				}
			}
			return 0.0f;
		}
	};
	/// <summary>
	/// ボーンのワールドマトリクスを持ってくる
	/// </summary>
	/// <param name="bone">マトリクスの欲しいボーン</param>
	/// <param name="worldMat">モデルのマトリクス</param>
	/// <returns></returns>
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
	/// <summary>
	/// 
	/// </summary>
	/// <param name="bone"></param>
	/// <param name="wm"></param>
	/// <param name="worldMat"></param>
	/// <returns></returns>
	CMatrix GetBoneLocalMatrix(Bone * bone, const CMatrix& wm, const CMatrix& worldMat)
	{
		CMatrix mat = wm;
		CMatrix inv;
		inv.Inverse(GetBoneWorldMatrix(bone->GetParent(),worldMat));
		mat.Mul(wm, inv);
		return mat;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="effector"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	float GetLengthEffector2End(Bone* effector, Bone* end)
	{
		float len = 0.f;
		CVector3 vlen = CVector3::Zero();
		Bone* pb=effector->GetParent();
		for (Bone* cb = effector; pb != end; pb = cb->GetParent())
		{
			vlen += (cb->GetWorldMatrix().GetTranslation() - pb->GetWorldMatrix().GetTranslation());
			cb = pb;
		}
		for (Bone* b = effector; b != end; b = b->GetParent())
		{
			len += b->GetWorldMatrix().GetTranslation().Length();
		}
		return len;
	}


	IK::IK(Bone* effector, Bone* end, float radius, bool isOnRigitBody, const CVector3& pos) :
		m_effectorBone(effector),
		m_endBone(end),
		m_radius(radius),
		m_isUseRigidBody(isOnRigitBody)
	{
		m_collider.Create(radius);
		if(isOnRigitBody)
			InitRigidBody(pos);
	}

	IK::~IK()
	{
		if (m_isUseRigidBody)
		{
			Physics().RemoveRigidBody(m_rigidBody);
			m_rigidBody.Release();
		}
		m_collider.Release();
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
		switch (m_ikMode)
		{
		case enMode_Normal:
			UpdateTarget_Normal(worldMat);
			break;
		case enMode_Foot:
			UpdateTarget_Foot(worldMat);
			break;
		}
	}


	/// <summary>
	/// 
	/// </summary>
	/// <param name="worldMat"></param>
	void IK::UpdateTarget_Normal(const CMatrix& worldMat)
	{
		auto effmat = GetBoneWorldMatrix(m_effectorBone, worldMat);
		auto effpos = effmat.GetTranslation();
		auto currentBone = m_effectorBone->GetParent();						//作業中のボーン

		auto newpos = effpos + m_offset;	//移動先のポジション
		if (m_isSetNextTarget)
		{
			newpos = m_nextTarget;
			m_isSetNextTarget = false;
			m_isSetOldTarget = true;
		}

		auto oldpos = m_effectorBone->GetWorldMatrix().GetTranslation();	//移動前のポジション
		if (m_isFirst)
		{
			oldpos += worldMat.GetTranslation();
			m_isFirst = false;
		}
		else
		{
			oldpos = m_target;
		}
		if ((newpos - oldpos).Length() < FLT_EPSILON)
			return;

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
			auto norm = sr.hitNormal;
			norm.Normalize();
			auto meri = newpos - sr.hitPos;
			if (!m_isHit)
			{
				m_effectorBone->SetIsONGround(true);

				float rad = norm.Dot(meri);
				auto ntarget = newpos + sr.hitNormal * (-rad + m_radius);
				target = sr.hitPos + sr.hitNormal * (m_radius);
				target.Lerp(m_rubbing, ntarget, target);
				m_rubTarget = ntarget;
			}
			else
			{
				float rad = norm.Dot(meri);
				auto ntarget = newpos + sr.hitNormal * (-rad + m_radius);
				target.Lerp(m_rubbing, ntarget, m_target);
			}
		}
		else
		{
			m_effectorBone->SetIsONGround(false);
			if (m_isHit)
			{
				m_effectorBone->SetMomentum(newpos - oldpos);
			}
			else
			{
				m_effectorBone->SetMomentum(CVector3::Zero());
			}
		}

		m_target += (target - oldpos) * m_speed;

		m_isHit = sr.isHit;
		m_move = newpos - m_target;
		//auto o2n = m_target - oldpos;
		m_effectorBone->SetMove(m_move);
		m_offset = CVector3::Zero();
		if (m_isUseRigidBody)
			UpdateRigidBody(m_target);
	}


	/// <summary>
	/// 
	/// </summary>
	/// <param name="worldMat"></param>
	void IK::UpdateTarget_Foot(const CMatrix& worldMat)
	{
		auto effmat = GetBoneWorldMatrix(m_effectorBone, worldMat);
		auto effpos = effmat.GetTranslation();
		auto currentBone = m_effectorBone->GetParent();						//作業中のボーン
		auto modelpos = worldMat.GetTranslation();		//モデルのポジション

		auto orgpos = effpos - modelpos;//モデルのポジションからの距離
		orgpos += m_offset;
		CVector3 newpos;	//移動先のポジション
		if (m_isSetNextTarget)
		{
			newpos = m_nextTarget + m_offset;
			orgpos = newpos- modelpos;
			m_isSetNextTarget = false;
			m_isSetOldTarget = true;
		}
		else
			newpos = effpos + m_offset;

		//if (m_timer <= 0.5f)		//ボーンの移動量からターゲットを求めるやつ
		//{
		//	//effmat = GetBoneWorldMatrix(m_effectorBone, imat);
		//	//effpos = effmat.GetTranslation();
		//	//auto move = (effpos+m_offset) - m_oldNewTarget;
		//	//newpos = m_target + move;
		//}
		//else
		//{
		//	m_timer = 0.0f;
		//}
		//m_timer += gameTime()->GetDeltaTime();

		m_oldTarget = m_target;
		CVector3 oldpos;	//移動前のポジション
		if (m_isFirst)
		{
			oldpos = m_effectorBone->GetBindPoseMatrix().GetTranslation();
			oldpos += modelpos;
			m_isFirst = false;
		}
		else
		{
			oldpos = m_target;
		}

		m_animMove = orgpos - m_oldNewTarget;
		//重力処理。
		float time = gameTime()->GetDeltaTime();
		if ((m_animMove.y) <= m_radius*0.05f)	//下に移動している
		{
			m_gravitPow += m_gravity * time;
			m_effectorBone->SetIsDown(true);
		}
		else	//上に移動している
		{
			m_gravitPow = 0.f;
			m_effectorBone->SetIsDown(false);
		}
		newpos.y -= m_gravitPow;		//プラスグラビティ(マイナス)

		if ((newpos - oldpos).Length() < FLT_EPSILON)		//ほんまに移動してんのか？
			return;	//移動してないやん！！

		//oldpos.y += m_radius;
		CVector3 nowpos = oldpos;		//正直いらない
		CVector3 target = oldpos;		//真の移動先。
		btTransform bstart, bend;			
		bstart.setIdentity();
		bend.setIdentity();
		bool isHitGround = false;		//地面に当たった？


		if (fabsf(orgpos.y - m_oldNewTarget.y) > FLT_EPSILON)
		{
			SweepResultIK_Floor sr;
			sr.startPos = oldpos;
			sr.me = m_rigidBody.GetBody();

			bstart.setOrigin(btVector3(oldpos.x, oldpos.y, oldpos.z));
			//bend.setOrigin(btVector3(newpos.x, newpos.y, newpos.z));
			bend.setOrigin(btVector3(oldpos.x, newpos.y, oldpos.z));

			//target = newpos;//ターゲット
			Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
			if (sr.isHit)
			{
				target = oldpos;
				target.y = sr.hitPos.y + m_radius;
			}
			else
			{
				target.y = newpos.y;
				if (m_isHit)
				{
					m_effectorBone->SetMomentum(newpos - nowpos);
				}
				else
				{
					m_effectorBone->SetMomentum(CVector3::Zero());
				}
			}
			m_effectorBone->SetIsONGround(sr.isHit);
			isHitGround = sr.isHit;
			m_hitCollision = sr.hitCollision;
		}
		else
		{
			m_effectorBone->SetIsONGround(m_isHit);
			isHitGround = m_isHit;
		}
		


		if ((CVector3( oldpos.x, 0.f, oldpos.z) -CVector3( newpos.x, 0.f, newpos.z )).Length() > FLT_EPSILON)
		{
			//壁の判定。
			bstart.setOrigin(btVector3(oldpos.x, target.y, oldpos.z));
			bend.setOrigin(btVector3(newpos.x, target.y, newpos.z));
			CVector3 newpos2 = newpos;
			newpos2.y = target.y;
			SweepResultIK_Wall sr_w;
			sr_w.me = m_rigidBody.GetBody();
			sr_w.startPos = CVector3(oldpos.x, target.y, oldpos.z);
			Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr_w);
			if (sr_w.isHit)
			{
				//m_effectorBone->SetIsONGround(true);

				auto norm = sr_w.hitNormal;
				norm.Normalize();
				auto meri = newpos2 - sr_w.hitPos;

				float rad = norm.Dot(meri);
				auto ntar = newpos2 + sr_w.hitNormal * (-rad + m_radius);
				if (isHitGround)
					target.Lerp(m_rubbing, ntar, target);
				else
					target = ntar;
			}
			else
			{
				if (isHitGround)
					target.Lerp(m_rubbing, newpos2, target);
				else
					target = newpos2;
			}
			m_hitCollision = sr_w.hitCollision;
		}

#if 1
		if (fabsf(orgpos.y - m_oldNewTarget.y) > m_radius * 0.05f && fabsf(target.y - newpos.y) > FLT_EPSILON)
		{
			SweepResultIK_Floor sr;
			sr.startPos = target;
			sr.me = m_rigidBody.GetBody();

			bstart.setOrigin(btVector3(target.x, target.y, target.z));
			bend.setOrigin(btVector3(target.x, newpos.y, target.z));

			//target = newpos;//ターゲット
			Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
			if (sr.isHit)
			{
				target.y = sr.hitPos.y + m_radius;
			}
			else
			{
				target.y = newpos.y;
				if (m_isHit)
				{
					m_effectorBone->SetMomentum(newpos - nowpos);
				}
				else
				{
					m_effectorBone->SetMomentum(CVector3::Zero());
				}
			}
			m_effectorBone->SetIsONGround(sr.isHit);
			isHitGround = sr.isHit;
			m_hitCollision = sr.hitCollision;
		}
#endif

		//m_rubTarget += (target-oldpos)*m_speed;
		m_target += (target - nowpos) * m_speed;
		//m_target = target;

		//m_oldNewTarget = effpos+m_offset;
		
		m_isHit = isHitGround;
		//newpos.y += m_gravitPow;
		
		m_move = (orgpos+m_offset) - (m_target- modelpos);
		//m_move = (effpos + m_offset) - m_target;
		m_effectorBone->SetMove(m_move);
		m_oldNewTarget = orgpos;
		m_offset = CVector3::Zero();

		if (m_isUseRigidBody)
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
		//どこにもヒットしてない、もしくはNextTargetもされてなく、IKモードがFootじゃない場合必ず早期リターン
		if (!(m_isHit or m_isSetOldTarget) and m_ikMode!=IKMode::enMode_Foot)
			return;
		for (int i = 0; i < 3; i++)
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
				//float deg = CMath::RadToDeg(rad);

				
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
		m_isSetOldTarget = false;
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

	void IK::SetPos(const CVector3& p)
	{
		m_target = p;
		m_oldNewTarget = p;
		m_oldTarget = p;
		//m_nextTarget = p;
		m_isSetNextTarget = false;
		m_rubTarget = p;
		UpdateRigidBody(p);
	}
}