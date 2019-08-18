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
			::QueryPerformanceCounter((LARGE_INTEGER*)&m_begin);
		}
		double Stop()
		{
			::QueryPerformanceFrequency((LARGE_INTEGER*)&m_frec);
			::QueryPerformanceCounter((LARGE_INTEGER*)&m_end);
			m_sec = double(m_end - m_begin) / m_frec;
			m_millSec = m_sec * 1000.;
			m_micrSec = m_millSec * 1000.;

			return m_sec;
		}

		double GetSec()
		{
			return m_sec;
		}
		double GetMillSec()
		{
			return m_millSec;
		}
		double GetMicrSec()
		{
			return m_micrSec;
		}

	private:
		LONGLONG m_frec = 0;
		LONGLONG m_begin = 0;
		LONGLONG m_end = 0;

		double m_sec = 0.0;
		double m_millSec = 0.0;
		double m_micrSec = 0.0;
	};
}