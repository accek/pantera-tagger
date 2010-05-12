#include "../../../corpuslib_headers.h"
#include "Timer.h"

namespace Corpus
{
	namespace TextUtils
	{
		namespace Misc
		{

			CTimer::CTimer()
			{
				Clear();
			}

			CTimer::~CTimer()
			{
			}

			double CTimer::Start()
			{
				Clear();
				m_ctLastTimeStamp = clock();
				m_ctTime = 0;
				return m_ctTime;
			}

			double CTimer::Stop()
			{
				clock_t now = clock();
				m_ctTime += (double(now - m_ctLastTimeStamp))/CLOCKS_PER_SEC;
				m_ctLastTimeStamp = now;
				return m_ctTime;
			}

			double CTimer::Continue()
			{
				m_ctLastTimeStamp = clock();
				return m_ctTime;
			}

			void CTimer::Clear()
			{
				m_ctLastTimeStamp = 0;
				m_ctTime = 0;
			}

		} // namespace Misc
	} // namespace TextUtils
} // namespace Corpus
