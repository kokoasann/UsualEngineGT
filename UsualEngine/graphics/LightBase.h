#pragma once


namespace UsualEngine
{
	

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