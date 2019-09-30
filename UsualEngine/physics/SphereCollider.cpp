#include "PreCompile.h"
#include "SphereCollider.h"

namespace UsualEngine
{
	SphereCollider::~SphereCollider()
	{
		delete m_body;
	}
	void SphereCollider::Create(float radius)
	{
		m_body = new btSphereShape(radius);
	}
}