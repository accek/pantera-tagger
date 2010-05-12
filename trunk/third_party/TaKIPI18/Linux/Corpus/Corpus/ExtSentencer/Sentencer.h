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

#ifndef CORPUSLIB_EXTSENTENCER_SENTENCER_H
#define CORPUSLIB_EXTSENTENCER_SENTENCER_H

#include "../../corpuslib_headers.h"
#include "../typedefs.h"
#include "LowLevelSentencer.h"
#include "../Tokenisation/TokenReader.h"
#include "../Token.h"
#include "../Readers/Reader.h"
#include "../ResourceManager.h"
#include "SentenceReader.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		/** High-level sentencer API. Generates sentences consisting of CToken
		  * objects.
		  *
		  * NOTE: the output sentences contain unchanged encoding of their tokens.
		  * When used with the tokeniser, it should be CP1250 (reencoding must be
		  * switched off). */
		class CORPUS_API CSentencer: public CSentenceReader
		{
		public:
			/** Creates a new sentencer on top of a tokeniser. Note: to handle
			  * abbrevations correctly, the tokeniser needs to have reencoding
			  * switched off (so that it outputs in cp1250). */
			CSentencer(Tokenisation::CTokenReader &tokreader);
			
			~CSentencer();
			
			/** Returns the new sentence object. It needs to be freed after use
			  * with CSentenceReader::Free call. */
			CSentence *GetNext(bool eliminateSubstGerIgn = true);
			
		private:
			CLowLevelSentencer worker;
			Corpus::Tokenisation::CTokenReader *tokrdr;
			std::queue<CToken *> tokQueue;
			
			Corpus::CSentence *Detach(INTEGER numTokens, bool eliminateSubstGerIgn);
			
		}; // class CSentencer
	} // namespace ExtSentencer
} // namespace Corpus

#endif
