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

#ifndef CORPUS_TEXTUTILS_DUMP_H
#define CORPUS_TEXTUTILS_DUMP_H

#include "../../../corpuslib_headers.h"
#include "../../Corpus.h"
#include "../../Sentence.h"

namespace Corpus
{
	namespace TextUtils
	{
		namespace CorpusUtil
		{
			class CORPUS_API CDump
			{
			public:
				CDump(const char *fileName);
				~CDump(void);

				void Dump(Corpus::CSentence& sentence);
				void Dump(const char *str);

			private:
				// Strumie� wyj�ciowy, do dump'owania.
				std::ofstream outStream;
			};

		} // namespace CorpusUtil
	} // namespace TextUtils
} // namespace Corpus

#endif //TEXTUTILS_DUMP_H

