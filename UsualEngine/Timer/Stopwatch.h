#pragma once


namespace UsualEngine
{
	class Stopwatch
	{
	public:
		Stopwatch()
		{

		}
		~Stopwatch()
		{

		}

		void Start()
		{
			::QueryPerformanceCounter((LARGE_INTEGER*)&mBegin);
		}
		double Stop()
		{
			::QueryPerformanceFrequency((LARGE_INTEGER*)&mFrec);
			::QueryPerformanceCounter((LARGE_INTEGER*)&mEnd);
			mSec = double(mBegin - mEnd) / mFrec;
			mMillSec = mSec * 1000.;
			mMicrSec = mMillSec * 1000.;

			return mSec;
		}

		double GetSec()
		{
			return mSec;
		}
		double GetMillSec()
		{
			return mMillSec;
		}
		double GetMicrSec()
		{
			return mMicrSec;
		}

	private:
		LONGLONG mFrec = 0;
		LONGLONG mBegin = 0;
		LONGLONG mEnd = 0;

		double mSec = 0.0;
		double mMillSec = 0.0;
		double mMicrSec = 0.0;
	};
}