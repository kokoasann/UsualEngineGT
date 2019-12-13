#pragma once

#include "ICollider.h"
namespace UsualEngine
{
	class SphereCollider:public ICollider
	{
	public:
		//
		SphereCollider(){}
		//
		~SphereCollider();

		/// <summary>
		/// ŠJ•úB
		/// </summary>
		void Release();
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="radius"></param>
		void Create(float radius);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		btSphereShape* GetBody() const override
		{
			return m_body;
		}
	private:
		btSphereShape* m_body=nullptr;			//
	};
}