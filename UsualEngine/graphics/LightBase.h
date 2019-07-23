#pragma once


namespace UsualEngine
{
	struct SDirectionLight
	{
		CVector3 dir = {0,-1,0};
		CVector4 color = CVector4::White();
	};

	class LightBase :public GameObject
	{
	protected:
		LightBase() {};
		~LightBase() {};
	public:
		bool Start() override final;
		void OnDestroy() override final;

		
	private:
	
	};
}