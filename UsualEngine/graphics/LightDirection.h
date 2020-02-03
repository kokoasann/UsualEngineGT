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
			return m_light;
		}

		//���̎w�������Z�b�g����
		void SetDir(CVector3 dir)
		{
			m_light.dir = dir;
		}

		//�P���̓��������Q�b�g����
		CVector3 GetDir()
		{
			return m_light.dir;
		}


		//���̐F���Z�b�g����
		void SetCol(CVector4 col)
		{
			m_light.color = col;
		}

		//���̐F���Q�b�g����
		CVector4 GetCol()
		{
			return m_light.color;
		}

	private:
		SDirectionLight m_light;		//DirectionLight�̍\����
	};
}