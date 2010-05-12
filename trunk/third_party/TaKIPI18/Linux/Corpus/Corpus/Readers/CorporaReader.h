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

#ifndef CORPUSLIB_CORPORAREADER_H
#define CORPUSLIB_CORPORAREADER_H

#include "../../corpuslib_headers.h"
#include "../Token.h"
#include "Reader.h"

namespace Corpus
{
	namespace Readers
	{
		class CORPUS_API CCorporaReader :
			public CReader
		{
		public:
			CCorporaReader(std::istream& stream, bool dissambOnly = false);
			~CCorporaReader(void);

			CToken* GetToken();

		private:
			bool m_bDissambOnly;
		};
	}
}

#endif

