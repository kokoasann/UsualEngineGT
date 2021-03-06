/*!
 * @brief	アニメータークラス。
 */


#include "PreCompile.h"
#include "graphics/animation/Animation.h"
#include "graphics/skeleton.h"
#include "graphics/skinModel.h"

namespace UsualEngine
{
	Animation::Animation()
	{
	}
	Animation::~Animation()
	{
		Release();
	}

	void Animation::Release()
	{
		for (auto& ik : m_ik)
		{
			delete ik;
		}
		m_ik.clear();
		//m_skeleton->Release();
	}

	void Animation::Init(SkinModel& skinModel, AnimationClip animClipList[], int numAnimClip)
	{
		if (animClipList == nullptr) {
#ifdef _DEBUG
			char message[256];
			strcpy(message, "animClipListがNULLです。\n");
			OutputDebugStringA(message);
			//止める。
			std::abort();
#endif

		}
		m_skeleton = &skinModel.GetSkeleton();
		m_animationClips.clear();

		for (int i = 0; i < numAnimClip; i++) {
			m_animationClips.push_back(&animClipList[i]);
		}
		for (auto& ctr : m_animationPlayController) {
			ctr.Init(m_skeleton);
		}
		

		Play(0);
	}
	/*!
	* @brief	ローカルポーズの更新。
	*/
	void Animation::UpdateLocalPose(float deltaTime)
	{
		m_interpolateTime += deltaTime;
		if (m_interpolateTime >= 1.0f) {
			//補間完了。
			//現在の最終アニメーションコントローラへのインデックスが開始インデックスになる。
			m_startAnimationPlayController = GetLastAnimationControllerIndex();
			m_numAnimationPlayController = 1;
			m_interpolateTime = 1.0f;
		}
		//AnimationPlayController::Update関数を実行していく。
		for (int i = 0; i < m_numAnimationPlayController; i++) {
			int index = GetAnimationControllerIndex(m_startAnimationPlayController, i);
			m_animationPlayController[index].Update(deltaTime, this);
		}
	}

	void Animation::UpdateGlobalPose()
	{
		//グローバルポーズ計算用のメモリをスタックから確保。
		int numBone = m_skeleton->GetNumBones();
		CQuaternion* qGlobalPose = (CQuaternion*)alloca(sizeof(CQuaternion) * numBone);
		CVector3* vGlobalPose = (CVector3*)alloca(sizeof(CVector3) * numBone);
		CVector3* vGlobalScale = (CVector3*)alloca(sizeof(CVector3) * numBone);
		for (int i = 0; i < numBone; i++) {
			qGlobalPose[i] = CQuaternion::Identity();
			vGlobalPose[i] = CVector3::Zero();
			vGlobalScale[i] = CVector3::One();
		}
		//グローバルポーズを計算していく。
		int startIndex = m_startAnimationPlayController;
		for (int i = 0; i < m_numAnimationPlayController; i++) {
			int index = GetAnimationControllerIndex(startIndex, i);
			float intepolateRate = m_animationPlayController[index].GetInterpolateRate();
			const auto& localBoneMatrix = m_animationPlayController[index].GetBoneLocalMatrix();
			for (int boneNo = 0; boneNo < numBone; boneNo++) {
				//平行移動の補完
				CMatrix m = localBoneMatrix[boneNo];
				vGlobalPose[boneNo].Lerp(
					intepolateRate,
					vGlobalPose[boneNo],
					*(CVector3*)m.m[3]
				);
				//平行移動成分を削除。
				m.m[3][0] = 0.0f;
				m.m[3][1] = 0.0f;
				m.m[3][2] = 0.0f;

				//拡大成分の補間。
				CVector3 vBoneScale;
				vBoneScale.x = (*(CVector3*)m.m[0]).Length();
				vBoneScale.y = (*(CVector3*)m.m[1]).Length();
				vBoneScale.z = (*(CVector3*)m.m[2]).Length();

				vGlobalScale[boneNo].Lerp(
					intepolateRate,
					vGlobalScale[boneNo],
					vBoneScale
				);
				//拡大成分を除去。
				m.m[0][0] /= vBoneScale.x;
				m.m[0][1] /= vBoneScale.x;
				m.m[0][2] /= vBoneScale.x;

				m.m[1][0] /= vBoneScale.y;
				m.m[1][1] /= vBoneScale.y;
				m.m[1][2] /= vBoneScale.y;

				m.m[2][0] /= vBoneScale.z;
				m.m[2][1] /= vBoneScale.z;
				m.m[2][2] /= vBoneScale.z;

				//回転の補完
				CQuaternion qBone;
				qBone.SetRotation(m);
				qGlobalPose[boneNo].Slerp(intepolateRate, qGlobalPose[boneNo], qBone);
			}
		}


		//グローバルポーズをスケルトンに反映させていく。
		for (int boneNo = 0; boneNo < numBone; boneNo++) {
			//oldSkelMat.push_back(m_skeleton->GetBone(boneNo)->GetWorldMatrix());


			//拡大行列を作成。
			CMatrix scaleMatrix;
			scaleMatrix.MakeScaling(vGlobalScale[boneNo]);
			//回転行列を作成。
			CMatrix rotMatrix;
			rotMatrix.MakeRotationFromQuaternion(qGlobalPose[boneNo]);
			//平行移動行列を作成。
			CMatrix transMat;
			transMat.MakeTranslation(vGlobalPose[boneNo]);

			

			//全部を合成して、ボーン行列を作成。
			CMatrix boneMatrix;
			boneMatrix.Mul(scaleMatrix, rotMatrix);
			boneMatrix.Mul(boneMatrix, transMat);


			m_skeleton->SetBoneLocalMatrix(
				boneNo,
				boneMatrix
			);
		}


		//最終アニメーション以外は補間完了していたら除去していく。
		int numAnimationPlayController = m_numAnimationPlayController;
		for (int i = 1; i < m_numAnimationPlayController; i++) {
			int index = GetAnimationControllerIndex(startIndex, i);
			if (m_animationPlayController[index].GetInterpolateRate() > 0.99999f) {
				//補間が終わっているのでアニメーションの開始位置を前にする。
				m_startAnimationPlayController = index;
				numAnimationPlayController = m_numAnimationPlayController - i;
			}
		}
		m_numAnimationPlayController = numAnimationPlayController;
	}


	void Animation::UpdateIKTarget()
	{
		for (auto ik : m_ik)
		{
			if (ik->IsActive())
			{
				ik->UpdateTarget(m_worldMatrix);
				ik->Update(m_worldMatrix);
			}
		}
	}

	void Animation::UpdateIK()
	{
		for (auto ik : m_ik)
		{
			if(ik->IsActive())
				ik->Update(m_worldMatrix);
		}
	}

	void Animation::UpdateContactIK()
	{
		for (auto ik : m_ik)
		{
			if(ik->IsActive() and ik->IsHit())
				ik->Update(m_worldMatrix);
		}
	}



	void Animation::Update(float deltaTime)
	{
		if (m_numAnimationPlayController == 0) {
			return;
		}
		//ローカルポーズの更新をやっていく。
		UpdateLocalPose(deltaTime);

		//グローバルポーズを計算していく。
		UpdateGlobalPose();

		//IK no Update
		//UpdateIK();
	}

	IK* Animation::FindIK(Bone* bone)
	{
		for (auto ik : m_ik)
		{
			if (bone == ik->GetEffectorBone())
			{
				return ik;
			}
		}
		return nullptr;
	}

	void Animation::SetIKMode(int mode, Bone* bone)
	{
		if (bone == nullptr)
		{
			for (auto ik : m_ik)
			{
				ik->SetIKMode((IK::IKMode)mode);
			}
		}
		else
		{
			for (auto ik : m_ik)
			{
				if (bone == ik->GetEffectorBone())
				{
					ik->SetIKMode((IK::IKMode)mode);
					return;
				}
			}
		}
	}

	void Animation::SetIKRub(float f, Bone* bone)
	{
		if (bone == nullptr)
		{
			for (auto ik : m_ik)
			{
				ik->SetRubbing(f);
			}
		}
		else
		{
			for (auto ik : m_ik)
			{
				if (bone == ik->GetEffectorBone())
				{
					ik->SetRubbing(f);
					return;
				}
			}
		}
	}

	void Animation::SetIKOffset(const CVector3& ofs, Bone* bone)
	{
		if (bone == nullptr)
		{
			for (auto ik : m_ik)
			{
				ik->SetOffset(ofs);
			}
		}
		else
		{
			for (auto ik : m_ik)
			{
				if (bone == ik->GetEffectorBone())
				{
					ik->SetOffset(ofs);
					return;
				}
			}
		}
	}
	void Animation::SetIKSpeed(float speed, Bone* bone)
	{
		if (bone == nullptr)
		{
			for (auto ik : m_ik)
			{
				ik->SetSpeed(speed);
			}
		}
		else
		{
			for (auto ik : m_ik)
			{
				if (bone == ik->GetEffectorBone())
				{
					ik->SetSpeed(speed);
					return;
				}
			}
		}
	}
	void Animation::SetIKActive(bool b,Bone* bone)
	{
		if (bone == nullptr)
		{
			for (auto ik : m_ik)
			{
				ik->SetIsActive(b);
			}
		}
		else
		{
			for (auto ik : m_ik)
			{
				if (bone == ik->GetEffectorBone())
				{
					ik->SetIsActive(b);
					return;
				}
			}
		}
	}
	void Animation::SetIKNextTarget(const CVector3& tar, Bone* bone)
	{
		for (auto ik : m_ik)
		{
			if (bone == ik->GetEffectorBone())
			{
				ik->SetNextTarget(tar);
				return;
			}
		}
	}
	void Animation::SetIKGravity(float g, Bone* bone)
	{
		if (bone == nullptr)
		{
			for (auto ik : m_ik)
			{
				ik->SetGravity(g);
			}
		}
		else
		{
			for (auto ik : m_ik)
			{
				if (bone == ik->GetEffectorBone())
				{
					ik->SetGravity(g);
					return;
				}
			}
		}
	}
	void Animation::QueryIK(std::function<void(IK*)> ikfunc)
	{
		for (auto ik : m_ik)
		{
			ikfunc(ik);
		}
	}
}