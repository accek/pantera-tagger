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

#ifndef CORPUSLIB_TOKENISATION_TOKENISER_H
#define CORPUSLIB_TOKENISATION_TOKENISER_H

#include "../../corpuslib_headers.h"
#include "TokenReader.h"
#include "../Token.h"
#include "TokLayer.h"
#include "../Parser/Parser.h"

namespace Corpus
{
	namespace Tokenisation
	{
		/** The main tokeniser class. Reads data stream and produces CToken
		  * output. Input data stream is assumed to be encoded accorting to
		  * CConfig::inputEncoding. By default output tokens use CP1250
		  * (internal) encoding. This behaviour can be altered by passing
		  * reencode == true. This results in setting the output encoding to
		  * CConfig::inputEncoding; however, the output encoding can be set to
		  * anything else by SetOutputEncoding.
		  * By default tokeniser introduces new flex classes. When compliance
		  * with KIPI tagset is desired pass noNewTags == true. It produces
		  * similar behaviour to that of old framework's PlainTextReader.
		  */
		class CORPUS_API CTokeniser: public CTokenReader
		{
		public:
			/// Creates a new tokeniser with given input stream.
			CTokeniser(std::istream &co_inStream, bool useMorpher = true,
					   bool reencode = false, bool noNewTags = false);
			
			/// Default destructor.
			~CTokeniser();
			
			/// Fetches the next token. Use CTokenReader::Free to release its memory.
			CToken *GetNext();
			
			/** Reads the whole input and outputs tokens' XML to stdout
			  * (for debugging). */
			void ReadAndDump();
			
			/** Sets the output encoding and turns reencoding on. By default it
			  * is switched off so that output tokens are encoded in CP1250
			  * (internal encoding). */
			void SetOutputEncoding(Parser::Encoding outputEnc);
			
			/** Disables reencoding, i.e. returns to the default behaviour
			  * (output tokens will be encoded in CP1250). */
			void DisableReencoding();

		protected:
			//CTokLayer *upperLayer;
			boost::shared_ptr<CTokLayer> upperLayer;
			std::queue<CToken *> outputTokens;
			bool useMorfeusz;
			bool useReencoding;
			Parser::Encoding outputEncoding;

			/** Converts given pretoken to a sequence of tokens and feeds
			  * them to the outputToken queue. */
			void FeedToQueue(SPretoken &pretoken);
		};
	} // namespace Tokenisation
} // namespace Corpus

#endif
