#pragma once
#include "GBuffer.h"

namespace UsualEngine
{
	class PreRender
	{
	public:
		PreRender(){}
		~PreRender(){}
		void Init(float h,float w);
		void Render();
		GBuffer& GetGBuffer()
		{
			return m_gBuffer;
		}
	private:
		GBuffer m_gBuffer;
	};
}