#pragma once



namespace UsualEngine
{
#define MAX_DIRLIGHT 8
	class LightManager
	{
	private:
		LightManager();
	public:
		~LightManager();

		LightManager* Get()
		{
			static LightManager* ins = NULL;
			if (ins == NULL)
			{
				ins = new LightManager();
			}
			return ins;
		}

		void Init();

		void AddLight(LightBase* light);
		void RmvLight(LightBase* light);


		void Update();
		void Render();
	private:
		std::vector<LightDirection*> mCDirLight;
		SDirectionLight mSDirLights[MAX_DIRLIGHT];
	};
}