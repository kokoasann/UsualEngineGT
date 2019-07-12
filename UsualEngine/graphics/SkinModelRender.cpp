#include "PreCompile.h"
#include "SkinModelRender.h"

namespace UsualEngine
{
	void SkinModelRender::Update()
	{
		mAnimation.Update(gameTime()->GetDeltaTime());
		mSkinModel.UpdateWorldMatrix(mPosition, mRotation, mScale);
	}

	void SkinModelRender::Init(const wchar_t* path, AnimationClip* anims, int animCount, EnFbxUpAxis axis)
	{
		mSkinModel.Init(path, axis);
		if(anims != NULL)
			mAnimation.Init(mSkinModel, anims, animCount);
	}


	void SkinModelRender::Render()
	{
		Camera& cam = usualEngine()->GetMainCamera();
		mSkinModel.Draw(cam.GetViewMatrix(), cam.GetProjectionMatrix());
	}
}