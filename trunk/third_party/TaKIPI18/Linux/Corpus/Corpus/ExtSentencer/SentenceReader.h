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

#ifndef CORPUSLIB_EXTSENTENCER_SENTENCEREADER_H
#define CORPUSLIB_EXTSENTENCER_SENTENCEREADER_H

#include "../../corpuslib_headers.h"
#include "../Sentence.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		/** Base class for all the processors extracting sentences from any
		  * kind of input. **/
		class CORPUS_API CSentenceReader
		{
		public:
			virtual ~CSentenceReader();
			virtual CSentence *GetNext(bool eliminateSubstGerIgn = true) = 0;
			
			/// Relases memory occupied by sentence and its tokens.
			static void Free(Corpus::CSentence *sent);
		};
	} // namespace ExtSentencer
} // namespace Corpus

#endif
