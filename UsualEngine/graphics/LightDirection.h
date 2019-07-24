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
			return mLight;
		}

		//光の指す方をセットする
		void SetDir(CVector3 dir)
		{
			mLight.dir = dir;
		}

		//輝きの導く方をゲットする
		CVector3 GetDir()
		{
			return mLight.dir;
		}


		//光の色をセットする
		void SetCol(CVector4 col)
		{
			mLight.color = col;
		}

		//光の色をゲットする
		CVector4 GetCol()
		{
			return mLight.color;
		}

	private:
		SDirectionLight mLight;		//DirectionLightの構造体
	};
}