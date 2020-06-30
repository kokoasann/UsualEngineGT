#pragma once


namespace UsualEngine
{
	class LightPoint:public LightBase
	{
	public:
		LightPoint() {}
		~LightPoint() {}


		//�X�V
		void Update() override;

		//�{�̂��Q�b�g
		SPointLight& GetBody()
		{
			return m_light;
		}


		void SetPos(const CVector3& pos)
		{
			m_light.pos = pos;
		}

		const CVector3& GetPos() const
		{
			return m_light.pos;
		}

		//���̐F���Z�b�g����
		void SetCol(const CVector4& col)
		{
			m_light.color = col;
		}

		//���̐F���Q�b�g����
		const CVector4& GetCol() const
		{
			return m_light.color;
		}

		void SetDecay(float v)
		{
			m_light.decay = v;
		}
		void SetRadius(float v)
		{
			m_light.radius = v;
		}
	private:
		SPointLight m_light;
	};
}