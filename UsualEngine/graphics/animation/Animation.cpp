/*!
 * @brief	�A�j���[�^�[�N���X�B
 */


#include "PreCompile.h"
#include "graphics/animation/Animation.h"
#include "graphics/skeleton.h"
#include "graphics/skinModel.h"

namespace UsualEngine
{
	//IK�p�̓����蔻��̂��
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
			strcpy(message, "animClipList��NULL�ł��B\n");
			OutputDebugStringA(message);
			//�~�߂�B
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
		
		//IK����{�[�����Q�b�c
		int cont = 0;
		std::wstring wstik = L"IK";
		for (auto bone : m_skeleton->GetAllBone())
		{
			std::wstring wname = bone->GetName();
			if (std::wstring::npos != wname.find(wstik))
			{
				m_isIKBoneList[cont] = bone->GetNo();
				cont++;
			}
		}
		m_collider.Create(5, 5);

		Play(0);
	}
	/*!
	* @brief	���[�J���|�[�Y�̍X�V�B
	*/
	void Animation::UpdateLocalPose(float deltaTime)
	{
		m_interpolateTime += deltaTime;
		if (m_interpolateTime >= 1.0f) {
			//��Ԋ����B
			//���݂̍ŏI�A�j���[�V�����R���g���[���ւ̃C���f�b�N�X���J�n�C���f�b�N�X�ɂȂ�B
			m_startAnimationPlayController = GetLastAnimationControllerIndex();
			m_numAnimationPlayController = 1;
			m_interpolateTime = 1.0f;
		}
		//AnimationPlayController::Update�֐������s���Ă����B
		for (int i = 0; i < m_numAnimationPlayController; i++) {
			int index = GetAnimationControllerIndex(m_startAnimationPlayController, i);
			m_animationPlayController[index].Update(deltaTime, this);
		}
	}

	void Animation::UpdateGlobalPose()
	{
		//�O���[�o���|�[�Y�v�Z�p�̃��������X�^�b�N����m�ہB
		int numBone = m_skeleton->GetNumBones();
		CQuaternion* qGlobalPose = (CQuaternion*)alloca(sizeof(CQuaternion) * numBone);
		CVector3* vGlobalPose = (CVector3*)alloca(sizeof(CVector3) * numBone);
		CVector3* vGlobalScale = (CVector3*)alloca(sizeof(CVector3) * numBone);
		for (int i = 0; i < numBone; i++) {
			qGlobalPose[i] = CQuaternion::Identity();
			vGlobalPose[i] = CVector3::Zero();
			vGlobalScale[i] = CVector3::One();
		}
		//�O���[�o���|�[�Y���v�Z���Ă����B
		int startIndex = m_startAnimationPlayController;
		for (int i = 0; i < m_numAnimationPlayController; i++) {
			int index = GetAnimationControllerIndex(startIndex, i);
			float intepolateRate = m_animationPlayController[index].GetInterpolateRate();
			const auto& localBoneMatrix = m_animationPlayController[index].GetBoneLocalMatrix();
			for (int boneNo = 0; boneNo < numBone; boneNo++) {
				//���s�ړ��̕⊮
				CMatrix m = localBoneMatrix[boneNo];
				vGlobalPose[boneNo].Lerp(
					intepolateRate,
					vGlobalPose[boneNo],
					*(CVector3*)m.m[3]
				);
				//���s�ړ��������폜�B
				m.m[3][0] = 0.0f;
				m.m[3][1] = 0.0f;
				m.m[3][2] = 0.0f;

				//�g�听���̕�ԁB
				CVector3 vBoneScale;
				vBoneScale.x = (*(CVector3*)m.m[0]).Length();
				vBoneScale.y = (*(CVector3*)m.m[1]).Length();
				vBoneScale.z = (*(CVector3*)m.m[2]).Length();

				vGlobalScale[boneNo].Lerp(
					intepolateRate,
					vGlobalScale[boneNo],
					vBoneScale
				);
				//�g�听���������B
				m.m[0][0] /= vBoneScale.x;
				m.m[0][1] /= vBoneScale.x;
				m.m[0][2] /= vBoneScale.x;

				m.m[1][0] /= vBoneScale.y;
				m.m[1][1] /= vBoneScale.y;
				m.m[1][2] /= vBoneScale.y;

				m.m[2][0] /= vBoneScale.z;
				m.m[2][1] /= vBoneScale.z;
				m.m[2][2] /= vBoneScale.z;

				//��]�̕⊮
				CQuaternion qBone;
				qBone.SetRotation(m);
				qGlobalPose[boneNo].Slerp(intepolateRate, qGlobalPose[boneNo], qBone);
			}
		}

		std::vector<CMatrix> oldSkelMat;
		oldSkelMat.reserve(numBone);

		auto a = m_skeleton->GetBone(3);
		auto m = GetBoneWorldMatrix(a);
		m = GetBoneLocalMatrix(a, m);

		//�O���[�o���|�[�Y���X�P���g���ɔ��f�����Ă����B
		for (int boneNo = 0; boneNo < numBone; boneNo++) {
			oldSkelMat.push_back(m_skeleton->GetBone(boneNo)->GetWorldMatrix());


			//�g��s����쐬�B
			CMatrix scaleMatrix;
			scaleMatrix.MakeScaling(vGlobalScale[boneNo]);
			//��]�s����쐬�B
			CMatrix rotMatrix;
			rotMatrix.MakeRotationFromQuaternion(qGlobalPose[boneNo]);
			//���s�ړ��s����쐬�B
			CMatrix transMat;
			transMat.MakeTranslation(vGlobalPose[boneNo]);

			

			//�S�����������āA�{�[���s����쐬�B
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

		UpdateIK(oldSkelMat);

		//�ŏI�A�j���[�V�����ȊO�͕�Ԋ������Ă����珜�����Ă����B
		int numAnimationPlayController = m_numAnimationPlayController;
		for (int i = 1; i < m_numAnimationPlayController; i++) {
			int index = GetAnimationControllerIndex(startIndex, i);
			if (m_animationPlayController[index].GetInterpolateRate() > 0.99999f) {
				//��Ԃ��I����Ă���̂ŃA�j���[�V�����̊J�n�ʒu��O�ɂ���B
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
		CMatrix parents[32];
		CMatrix mat = wm;
		int cont = 0;
		CMatrix inv;
		/*Bone* b = bone;
		while (true)
		{
			b = b->GetParent();
			if (b == nullptr)
				break;
			b->GetInvBindPoseMatrix();
			inv.Inverse(GetBoneWorldMatrix(b));
			mat.Mul(mat,inv);
		}*/
		inv.Inverse(GetBoneWorldMatrix(bone->GetParent()));
		mat.Mul(wm,inv);
		return mat;
	}

	void Animation::UpdateIK(const std::vector<CMatrix>& oldBonesMat)
	{
		const auto& bones = m_skeleton->GetAllBone();
		
		//
		for (int ind : m_isIKBoneList)
		{
			if (ind <= 0)
				break;
			auto effectorBone = bones[ind];			//
			auto effmat = GetBoneWorldMatrix(effectorBone);
			auto effpos = effmat.GetTranslation();
			auto currentBone = effectorBone->GetParent();						//��ƒ��̃{�[��
			
/*
			auto bone = bones.at(ind);
			Bone* parents[32];
			int cont = 0;
			while (bone != nullptr)
			{
				bone = bone->GetParent();
				parents[cont++] = bone;
			}
			auto mat = m_worldMatrix;
			for (int i = cont - 1; i <= 0; i++)
			{
				mat.Mul(mat, parents[i]->GetLocalMatrix());
			}
			mat.Mul(mat, bones[ind]->GetLocalMatrix());*/
			auto mat = GetBoneWorldMatrix(effectorBone);
			auto newpos = mat.GetTranslation();	//�ړ���̃|�W�V����

			//auto localmat = bones[ind]->GetLocalMatrix();
			//auto worldmat = CMatrix::Identity();
			//worldmat.Mul(m_worldMatrix,localmat);
			//auto newpos = worldmat.GetTranslation();	//�ړ���̃|�W�V����

			auto oldpos = oldBonesMat[ind].GetTranslation();	//�ړ��O�̃|�W�V����

			SweepResultIK sr;
			sr.startPos = oldpos;

			btTransform bstart, bend;
			bstart.setIdentity();
			bend.setIdentity();

			bstart.setOrigin(btVector3(oldpos.x, oldpos.y, oldpos.z));
			bend.setOrigin(btVector3(newpos.x, newpos.y, newpos.z));

			auto target = newpos;			//�^�[�Q�b�g
						//�^�[�Q�b�g
			g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
			if (sr.isHit or true)
			{
				auto norm = sr.hitNormal;
				norm.Normalize();
				auto meri = newpos - sr.hitPos;
				float rad = meri.Dot(norm);
				target += sr.hitNormal * (-rad + 5);

				auto invworldmat = CMatrix::Identity();
				invworldmat.Inverse(m_worldMatrix);
				invworldmat.Mul(target);

				auto target = CVector3{ 500,100,0 };
				std::wstring end = L"END";
				std::wstring wname = currentBone->GetName();
				for (int i = 0; i < 2; i++)
				{
					while (true)
					{
						auto currentmat = GetBoneWorldMatrix(currentBone);//��ƃ{�[���̐��E�s��B

						auto currentlocal = currentBone->GetLocalMatrix();//��ƃ{�[���̃��[�J���s��B
						CVector3 lopo, losc;
						CQuaternion loro;
						currentlocal.CalcMatrixDecompose(lopo, loro, losc);

						auto currentpos = currentmat.GetTranslation();	//��ƒ��̃{�[���̈ʒu�B

						CMatrix inv;
						inv.Inverse(currentmat);

						CVector3 localeffpos=effpos;
						inv.Mul(localeffpos);
						CVector3 localtarpos = target;
						inv.Mul(localtarpos);

						auto toEffector = localeffpos;			//�G�t�F�N�^�[����J�����g�{�[���̃x�N�g��
						auto toTarget = localtarpos;			//�^�[�Q�b�g����J�����g�{�[���̃x�N�g��

						auto elen = toEffector.Length();
						auto tlen = toTarget.Length();

						toEffector.Normalize();
						toTarget.Normalize();

						auto rad = toEffector.Dot(toTarget);			//��̃x�N�g���̊p�x(���b�h�E�B���v�X)
						rad = acos(rad);
						float deg = CMath::RadToDeg(rad);

						if (rad > 0.000001f)
						{
							auto axis = CVector3::Zero();					//��]��.
							axis.Cross(toEffector, toTarget);
							axis.Normalize();

							auto addrot = CQuaternion::Identity();			//�������].
							addrot.SetRotation(axis, rad);

							CMatrix mRot;									//�������]�s��B
							mRot.MakeRotationFromQuaternion(addrot);

							auto pos = CVector3::Zero();


							loro.Multiply(addrot);
							CMatrix msca, mrot, mpos,mfin;
							msca.MakeScaling(losc);
							mrot.MakeRotationFromQuaternion(loro);
							mpos.MakeTranslation(lopo);

							mfin.Mul(msca, mrot);
							mfin.Mul(mfin, mpos);
							currentBone->SetLocalMatrix(mfin);


							/*pos = currentlocal.GetTranslation();
							currentlocal.m[3][0] = 0.f;
							currentlocal.m[3][1] = 0.f;
							currentlocal.m[3][2] = 0.f;
							currentlocal.Mul(currentlocal, mRot);
							CMatrix tra;
							tra.MakeTranslation(pos);
							currentlocal.Mul(currentlocal, tra);

							currentBone->SetLocalMatrix(currentlocal);*/
						}

						/*static int cnt = 0;
						static int cntmax = 1;
						if (cnt == cntmax)
						{
							cnt = 0;
							break;
						}
						cnt++;*/

						if (std::wstring::npos != wname.find(L"END"))
							break;
						effmat = GetBoneWorldMatrix(effectorBone);
						effpos = effmat.GetTranslation();

						currentBone = currentBone->GetParent();
						wname = currentBone->GetName();
					}
				}
			}
			//break;//debug�p�B
		}
	}



	void Animation::Update(float deltaTime)
	{
		if (m_numAnimationPlayController == 0) {
			return;
		}
		//���[�J���|�[�Y�̍X�V������Ă����B
		UpdateLocalPose(deltaTime);

		//�O���[�o���|�[�Y���v�Z���Ă����B
		UpdateGlobalPose();
	}
}