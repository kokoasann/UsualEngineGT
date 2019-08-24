#pragma once

#include "graphics/SkinModelRender.h"

namespace UsualEngine
{
	class SkinModelRender4Ground:public SkinModelRender
	{
	public:
		struct GroundCB
		{
			CVector3 scale = CVector3::Zero();
		};
		/// <summary>
		/// 
		/// </summary>
		/// <param name="path"></param>
		/// <param name="anims"></param>
		/// <param name="animCount"></param>
		/// <param name="axis"></param>
		void InitG(const wchar_t* path, AnimationClip* anims = nullptr, int animCount = 0, EnFbxUpAxis axis = enFbxUpAxisZ);

		void Render();
	private:
		ConstantBuffer m_groundCB;		//�萔�o�b�t�@
		GroundCB m_goundData;			//�萔�o�b�t�@�̒��g�B
		Shader m_psShader;
	};
}