#include "PreCompile.h"
#include "SkinModelRender.h"

namespace UsualEngine
{
	void SkinModelRender::Update()
	{
		auto& ske = m_skinModel.GetSkeleton();

		m_animation.Update(gameTime()->GetDeltaTime());		//アニメーションの更新。
		
		ske.UpdateBase(CMatrix::Identity());				//回転のためのスケルトン更新。
		m_animation.UpdateIKTarget();						//IKのターゲットの位置を更新
		
		CVector3 opos = m_position;
		if (m_isUseRotateFunc)
		{
			m_rotateFunc(m_rotation);				//IKとかを利用した回転の処理。足を滑らせたく無い時とか使うといいとおもう。
		}
		if (m_isUseMoveFunc)
		{
			m_moveFunc(m_position);					//IKとかを利用した移動の処理。足を滑らせたく無い時とか使うといいとおもう。
		}
		

		//3dsMaxと軸を合わせるためのバイアス。
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
		//平行移動行列を作成する。
		transMatrix.MakeTranslation(m_position);
		//回転行列を作成する。
		rotMatrix.MakeRotationFromQuaternion(m_rotation);
		rotMatrix.Mul(mBias, rotMatrix);
		//拡大行列を作成する。
		scaleMatrix.MakeScaling(scale);
		//ワールド行列を作成する。
		//拡大×回転×平行移動の順番で乗算するように！
		//順番を間違えたら結果が変わるよ。
		CMatrix worldMatrix = CMatrix::Identity(), scarot;
		scarot.Mul(scaleMatrix, rotMatrix);
		worldMatrix.Mul(scarot, transMatrix);

		//m_animation.SetWorldMatrix(worldMatrix);

		if (1)
		{
			m_animation.SetWorldMatrix(worldMatrix);
			m_animation.UpdateIK();
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