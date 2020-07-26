#pragma once
#include "SkyRender.h"

namespace UsualEngine
{
	class GameObject;
	class GBuffer
	{
	public:
		enum GBufferIndex
		{
			GB_Diffuse,
			GB_Specular,
			GB_Depth,
			GB_Normal,
			GB_Shadow,
			GB_Tangent,
			GB_Num
		};
		GBuffer() {}
		~GBuffer() {}
		void Init(float fh,float fw);
		void AddSkinModel(GameObject* model)
		{
			m_modelList.push_back(model);
		}
		void SetGBuffer();
		void UnSetGBuffer();
		void Draw();
		RenderTarget* GetGBuffer(GBufferIndex ind)
		{
			return &m_gBuffer[ind];
		}
	private:
		RenderTarget m_gBuffer[GB_Num];
		std::vector<GameObject*> m_modelList;

		SkyRender m_sky;
	};
}