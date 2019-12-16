#pragma once

#include "ICollider.h"

namespace UsualEngine
{
	class BoxCollider :public ICollider
	{
	public:
		~BoxCollider()
		{
			Release();
		}
		void Release()
		{
			delete m_body;
		}
		void Create(const CVector3& size)
		{
			m_body = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
			
		}

		virtual btCollisionShape* GetBody() const override
		{
			return m_body;
		}
	private:
		btBoxShape* m_body = nullptr;

		
	};
}