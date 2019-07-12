#pragma once


namespace UsualEngine
{
	class GameTime
	{
	
		GameTime() {}
		~GameTime(){}
	public:

		static GameTime* Get()
		{
			static GameTime* inst = nullptr;
			if (inst == nullptr)
				inst = new GameTime();
			return inst;
		}

		void PushFrameDeltaTime(float time)
		{
			mDeltaTimeQue.push_back(time);
			float Qsize = mDeltaTimeQue.size();
			if (Qsize > 30)
			{
				float totalTime = 0;
				for (auto dt : mDeltaTimeQue)
				{
					totalTime += dt;
				}
				mFrameDeltaTime = min(1.f / 30.f, totalTime / Qsize);
				mDeltaTimeQue.pop_front();
			}
		}

		float GetDeltaTime()
		{
			return mFrameDeltaTime;
		}

	private:
		std::list<float> mDeltaTimeQue;
		float mFrameDeltaTime = 1.f / 30.f;
	};

	static GameTime* gameTime()
	{
		return GameTime::Get();
	}
}