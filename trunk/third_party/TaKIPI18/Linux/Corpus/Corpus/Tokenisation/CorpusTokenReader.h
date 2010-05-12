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

#ifndef CORPUSLIB_TOKENISATION_CORPUSTOKENREADER_H
#define CORPUSLIB_TOKENISATION_CORPUSTOKENREADER_H

#include "../../corpuslib_headers.h"
#include "TokenReader.h"
#include "../Token.h"
#include "../Parser/Parser.h"

namespace Corpus
{
	namespace Tokenisation
	{
		/** Corpus token reader should be used to read KIPI morph.xml files.
		  * It is still a token reader, i.e. it reads only tokens, discarding
		  * all other information (e.g. sentence division and other chunk
		  * markers). CCorpusTokenReader is a replacement of old reader
		  * framework's CCorporaReader.
		  * Remember to dispose of output tokens after use by calling
		  * CTokenReader::Free (however in case they are passed to a sentencer,
		  * just free sentence objects instead as sentences manage their token
		  * memory themselves). */
		class CORPUS_API CCorpusTokenReader: public CTokenReader
		{
		public:
			/** Creates a corpus reader. Input tokens are assumed to be
			  * in Corpus::CConfig::inputEncoding. The default behaviour is to
			  * encode output tokens in CP1250. This can be altered by passing
			  * convertToCP = false (then outputEnc == inputEnc). */
			CCorpusTokenReader(std::istream &co_inStream, bool disambOnly = false, bool convertToCP = true);
			~CCorpusTokenReader();
			CToken *GetNext();

		private:
			bool filterDisamb, convertOutput;
			
			std::string GetNextTokenText(std::istream &co_inStream);
		};
	} // namespace Tokenisation
} // namespace Corpus

#endif
