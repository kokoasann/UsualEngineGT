/*!
 * @brief	アニメータークラス。
 */


#include "PreCompile.h"
#include "graphics/animation/Animation.h"
#include "graphics/skeleton.h"
#include "graphics/skinModel.h"

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

		btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character)
			{
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

	Animation::Animation()
	{
	}
	Animation::~Animation()
	{

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

		for (int i = 0; i < numAnimClip; i++) {
			m_animationClips.push_back(&animClipList[i]);
		}
		for (auto& ctr : m_animationPlayController) {
			ctr.Init(m_skeleton);
		}
		
		//IKするボーンをゲッツ
		int cont = 0;
		std::wstring wstik = L"IK";
		for (auto bone : m_skeleton->GetAllBone())
		{
			std::wstring wname = bone->GetName();
			if (std::wstring::npos != wname.find(wstik))
			{
				m_isIKBoneList[cont] = bone->GetNo();

				/*SkinModelRender* s = NewGO<SkinModelRender>(0);
				s->Init(L"Assets/model/dun.cmo");
				s->SetSca({ 0.05,0.05,0.05 });
				m_marker.push_back(s);*/

				cont++;
			}
		}
		m_collider.Create(80);


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

		//std::vector<CMatrix> oldSkelMat;
		//oldSkelMat.reserve(numBone);


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

			
			/*if (boneNo == 3)
			{
				auto rot = CQuaternion::Identity();
				rot.SetRotationDeg(CVector3::AxisZ(), 60);
				CMatrix r;
				r.MakeRotationFromQuaternion(rot);
				boneMatrix.Mul(boneMatrix, rotMatrix);
			}*/

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

	struct BoneMatrix
	{
		CMatrix World;
		CMatrix Local;
	};

	void Animation::UpdateIK()
	{
		for (auto ik : m_ik)
		{
			ik->Update(m_worldMatrix);
		}
	}

	CMatrix Animation::GetBoneWorldMatrix(Bone* bone)
	{
		Bone* parents[32] = {NULL};
		int cont = 0;
		Bone* b = bone;
		while (true)
		{
			parents[cont++] = b;
			b = b->GetParent();
			if (b == nullptr)
				break;
		}
		auto mat = m_worldMatrix;
		for (int i = cont - 1; i >= 0; i--)
		{
			auto locmat = parents[i]->GetLocalMatrix();
			mat.Mul(locmat,mat);
		}
		//mat.Mul(bone->GetLocalMatrix(),mat);
		return mat;
	}

	CMatrix Animation::GetBoneLocalMatrix(Bone* bone,CMatrix wm)
	{
		CMatrix mat = wm;
		CMatrix inv;
		inv.Inverse(GetBoneWorldMatrix(bone->GetParent()));
		mat.Mul(wm,inv);
		return mat;
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
		UpdateIK();
	}
}