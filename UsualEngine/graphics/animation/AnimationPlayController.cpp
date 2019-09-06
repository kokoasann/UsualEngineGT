/*!
 * @brief	アニメーション再生コントローラ。
 */

#include "PreCompile.h"
#include "graphics/animation/AnimationPlayController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/skeleton.h"


	
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
	struct Node
	{
		CVector3 pos;
		float rad = 0;
	};
	struct Joint
	{
		CVector3 pos;
		float n = 0;
	};

	void AnimationPlayController::Init(Skeleton* skeleton)
	{
		int numBones = skeleton->GetNumBones();
		m_skeleton = skeleton;
		//ボーン行列をバシッと確保。
		m_boneMatrix.resize(numBones);

		//IKするボーンをゲッツ
		int cont = 0;
		for (auto bone : skeleton->GetAllBone())
		{
			if (std::wstring::npos != std::wstring(L"IK").find(bone->GetName()))
			{
				m_isIKBoneList[cont] = bone->GetNo();
				cont++;
			}
		}
		m_collider.Create(5, 5);
	}



	void AnimationPlayController::StartLoop()
	{
		m_currentKeyFrameNo = 0;
		m_time = 0.0f;
	}
	void AnimationPlayController::Update(float deltaTime, Animation* animation)
	{
		if (m_animationClip == nullptr) {

			return;
		}
		const auto& topBoneKeyFrameList = m_animationClip->GetTopBoneKeyFrameList();
		m_time += deltaTime;

		//補完時間も進めていく。
		m_interpolateTime = min(1.0f, m_interpolateTime + deltaTime);
		while (true) {
			if (m_currentKeyFrameNo >= (int)topBoneKeyFrameList.size()) {
				//終端まで行った。
				if (m_animationClip->IsLoop()) {
					//ループ。
					StartLoop();
				}
				else {
					//ワンショット再生。
					m_currentKeyFrameNo--;
					m_isPlaying = false;	//再生終わり。
				}
				break;
			}
			if (topBoneKeyFrameList.at(m_currentKeyFrameNo)->time >= m_time) {
				//終わり。
				break;
			}
			//次へ。
			m_currentKeyFrameNo++;
		}


		//ボーン行列を計算していく。
		const auto& keyFramePtrListArray = m_animationClip->GetKeyFramePtrListArray();
		//
		for (int ind : m_isIKBoneList)
		{
			int i = ind;
			const auto& kfl = keyFramePtrListArray[ind];
			auto keyFrame = kfl[m_currentKeyFrameNo];
			auto oldWBM = CMatrix::Identity(),WBM = CMatrix::Identity();
			m_worldMatrix.Mul(m_worldMatrix, keyFrame->transform);
			CVector3 vert = m_worldMatrix.GetTranslation();
			
			
			Bone* currentBone = m_skeleton->GetAllBone()[ind];
			auto vertold = currentBone->GetWorldMatrix().GetTranslation();
			auto v = vert - vertold;

			SweepResultIK sr;
			sr.startPos = vertold;
			btTransform bstart, bend;
			bstart.setIdentity();
			bend.setIdentity();
			bstart.setOrigin(btVector3(vertold.x, vertold.y, vertold.z));
			bend.setOrigin(btVector3(vert.x, vert.y, vert.z));
			g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
			if (sr.isHit)
			{
				auto norm = sr.hitNormal;
				norm.Normalize();
				auto meri = vert - sr.hitPos;
				float rad = meri.Dot(norm);
				vert += sr.hitNormal * (-rad + 5);


				std::wstring end = L"END";
				currentBone = m_skeleton->GetAllBone()[ind]->GetParent();
				while(std::wstring::npos != end.find(currentBone->GetName()))
				{
					Node n;
					n.pos = vert - currentBone->GetWorldMatrix().GetTranslation();
					Joint j;
					
				}
			}
		}

		for (const auto& keyFrameList : keyFramePtrListArray) {
			if (keyFrameList.size() == 0) {
				continue;
			}
			//現在再生中のキーフレームを取ってくる。
			Keyframe* keyframe = keyFrameList.at(m_currentKeyFrameNo);
			if (keyframe->boneIndex < m_boneMatrix.size()) {
				m_boneMatrix[keyframe->boneIndex] = keyframe->transform;

			}
			else {
#ifdef _DEBUG			
				MessageBox(NULL, "AnimationPlayController::Update : 存在しないボーンに値を書き込もうとしています。次のような原因が考えられます。\n"
					"① tkaファイルを出力する時に、選択したルートボーンがスケルトンのルートボーンと異なっている。\n"
					"② 異なるスケルトンのアニメーションクリップを使っている。\n"
					"もう一度tkaファイルを出力しなおしてください。", "error", MB_OK);
				std::abort();
#endif
			}
		}
	}

}