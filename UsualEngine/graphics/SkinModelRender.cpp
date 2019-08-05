#include "PreCompile.h"
#include "SkinModelRender.h"

namespace UsualEngine
{
	void SkinModelRender::Update()
	{
		m_animation.Update(gameTime()->GetDeltaTime());
		m_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}

	void SkinModelRender::Init(const wchar_t* path, AnimationClip* anims, int animCount, EnFbxUpAxis axis)
	{
		m_skinModel.Init(path, axis);
		if(anims != NULL)
			m_animation.Init(m_skinModel, anims, animCount);
	}


	void SkinModelRender::Render()
	{
		Camera& cam = usualEngine()->GetMainCamera();
		m_skinModel.Draw(cam.GetViewMatrix(), cam.GetProjectionMatrix());
	}
}