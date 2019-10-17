#include "PreCompile.h"
#include "SkinModelRender.h"

namespace UsualEngine
{
	void SkinModelRender::Update()
	{
		//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
		CMatrix mBias = CMatrix::Identity();
		CVector3 scale = m_scale;
		if (m_skinModel.GetFbxUpAxis() == enFbxUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(CMath::PI * -0.5f);
			float z = scale.z;
			scale.z = scale.y;
			scale.y = z;
		}
		CMatrix transMatrix, rotMatrix, scaleMatrix;
		//���s�ړ��s����쐬����B
		transMatrix.MakeTranslation(m_position);
		//��]�s����쐬����B
		rotMatrix.MakeRotationFromQuaternion(m_rotation);
		rotMatrix.Mul(mBias, rotMatrix);
		//�g��s����쐬����B
		scaleMatrix.MakeScaling(scale);
		//���[���h�s����쐬����B
		//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
		//���Ԃ��ԈႦ���猋�ʂ��ς���B
		CMatrix worldMatrix = CMatrix::Identity(),scarot;
		scarot.Mul(scaleMatrix, rotMatrix);
		worldMatrix.Mul(scarot, transMatrix);

		m_animation.SetWorldMatrix(worldMatrix);
		m_animation.Update(gameTime()->GetDeltaTime());

		auto& ske = m_skinModel.GetSkeleton();
		
		ske.UpdateBase(worldMatrix);
		
		if (m_isUseMoveFunc)
		{
			m_moveFunc(m_position);
		}
		if (m_isUseRotateFunc)
		{
			m_rotateFunc(m_rotation);
		}
		
		m_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		
		
		m_isRenderingOK = true;
	}

	void SkinModelRender::Init(const wchar_t* path, AnimationClip* anims, int animCount, EnFbxUpAxis axis)
	{
		m_skinModel.Init(path, axis);
		if(anims != NULL)
			m_animation.Init(m_skinModel, anims, animCount);
	}


	void SkinModelRender::Render()
	{
		if (!m_isRenderingOK)
			return;
		Camera& cam = usualEngine()->GetMainCamera();
		m_skinModel.Draw(cam.GetViewMatrix(), cam.GetProjectionMatrix());
	}
}