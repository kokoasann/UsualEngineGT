#pragma once

#include "LightBase.h"
#include "LightStruct.h"

namespace UsualEngine
{
	//ディレクションライト
	class LightDirection:public LightBase
	{
	public:
		LightDirection();
		~LightDirection();


		//更新
		void Update() override;

		//本体をゲット
		SDirectionLight& GetBody()
		{
			return m_light;
		}

		//光の指す方をセットする
		void SetDir(CVector3 dir)
		{
			m_light.dir = dir;
		}

		//輝きの導く方をゲットする
		CVector3 GetDir()
		{
			return m_light.dir;
		}


		//光の色をセットする
		void SetCol(CVector4 col)
		{
			m_light.color = col;
		}

		//光の色をゲットする
		CVector4 GetCol()
		{
			return m_light.color;
		}

	private:
		SDirectionLight m_light;		//DirectionLightの構造体
	};
}