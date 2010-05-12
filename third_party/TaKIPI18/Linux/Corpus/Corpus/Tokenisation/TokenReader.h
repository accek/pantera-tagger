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

#ifndef CORPUSLIB_TOKENISATION_TOKENREADER_H
#define CORPUSLIB_TOKENISATION_TOKENREADER_H

#include "../../corpuslib_headers.h"
#include "../Token.h"

namespace Corpus
{
	namespace Tokenisation
	{
		/** Base class for all the processors extracting tokens from any kind
		  * of input stream.
		  *
		  * Convention: strings encoded in cp1250 (internal Corpus lib
		  * encoding) are prefixed with cp_, while strings encoded in
		  * CConfig::inputEncoding (or another external encoding if
		  * explicitly set by the user) are prefixed with co_.
		  **/
		class CORPUS_API CTokenReader
		{
		public:
			CTokenReader(std::istream &co_inStream);
			virtual ~CTokenReader(void);
			virtual CToken *GetNext() = 0;
			
			/** Releases the memory used by CToken output by the tokeniser
			  * or any other TokenReader. */
			static void Free(CToken *token);

		protected:
			std::istream &co_input;
		};
	} // namespace Tokenisation
} // namespace Corpus

#endif
