#include "PreCompile.h"
#include "SpriteRender.h"


namespace UsualEngine
{
	void SpriteRender::OnDestroy()
	{
	}
	void SpriteRender::Init(const wchar_t* path, CVector2 imageSize)
	{
		mSprite.Init(path, imageSize);
	}
	void SpriteRender::Update()
	{
		mSprite.Update(mPosition, mRotation, mScale, mPivot);
	}
	void SpriteRender::PostRender()
	{
		Camera& cam = usualEngine()->Get2DCamera();
		mSprite.Draw(cam.GetViewMatrix(), cam.GetProjectionMatrix());
	}
}