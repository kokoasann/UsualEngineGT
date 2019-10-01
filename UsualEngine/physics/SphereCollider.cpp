#include "PreCompile.h"
#include "SphereCollider.h"

namespace UsualEngine
{
	SphereCollider::~SphereCollider()
	{
		if(m_body!=nullptr)
			delete m_body;
	}
	void SphereCollider::Create(float radius)
	{
		m_body = new btSphereShape(radius);
	}
}