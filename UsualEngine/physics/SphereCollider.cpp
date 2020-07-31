#include "PreCompile.h"
#include "SphereCollider.h"

namespace UsualEngine
{
	SphereCollider::~SphereCollider()
	{
		Release();
	}
	void SphereCollider::Release()
	{
		if (m_body != nullptr)
			delete m_body;
		m_body = nullptr;
	}
	void SphereCollider::Create(float radius)
	{
		m_body = new btSphereShape(radius);
	}
}