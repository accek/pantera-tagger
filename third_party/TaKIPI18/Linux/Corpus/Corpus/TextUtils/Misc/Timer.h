/*  Biblioteka Corpus
 *  Zestaw narzędzi do przetwarzania tekstów w języku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef CORPUS_TEXTUTILS_TIMER_H
#define CORPUS_TEXTUTILS_TIMER_H

#include "../../../corpuslib_headers.h"

namespace Corpus
{
	namespace TextUtils
	{
		namespace Misc
		{

			class CORPUS_API CTimer
			{
				clock_t m_ctLastTimeStamp;
				double m_ctTime;

			public:
				CTimer(void);
				~CTimer(void);

				double Start();
				double Stop();
				double Continue();
				void Clear();
			};

		} // namespace Misc
	} // namespace TextUtils
} // namespace Corpus

#endif 
