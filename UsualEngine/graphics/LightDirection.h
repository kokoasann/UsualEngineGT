#pragma once

#include "LightBase.h"
#include "LightStruct.h"

namespace UsualEngine
{
	//�f�B���N�V�������C�g
	class LightDirection:public LightBase
	{
	public:
		LightDirection();
		~LightDirection();


		//�X�V
		void Update() override;

		//�{�̂��Q�b�g
		SDirectionLight& GetBody()
		{
			return mLight;
		}

		//���̎w�������Z�b�g����
		void SetDir(CVector3 dir)
		{
			mLight.dir = dir;
		}

		//�P���̓��������Q�b�g����
		CVector3 GetDir()
		{
			return mLight.dir;
		}


		//���̐F���Z�b�g����
		void SetCol(CVector4 col)
		{
			mLight.color = col;
		}

		//���̐F���Q�b�g����
		CVector4 GetCol()
		{
			return mLight.color;
		}

	private:
		SDirectionLight mLight;		//DirectionLight�̍\����
	};
}