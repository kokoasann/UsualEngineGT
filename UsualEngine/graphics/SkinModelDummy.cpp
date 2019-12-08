#include "PreCompile.h"
#include "SkinModelDummy.h"

namespace UsualEngine
{
	void SkinModelDummy::Update()
	{
		m_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}

	void SkinModelDummy::Init(const wchar_t* path, EnFbxUpAxis axis)
	{
		m_skinModel.Init(path, axis);
	}
}