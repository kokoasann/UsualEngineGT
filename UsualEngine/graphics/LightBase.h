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
		/// <summary>
		/// �J���B
		/// </summary>
		void Release() override{}
		void OnDestroy() override final;

		
	private:
	
	};
}