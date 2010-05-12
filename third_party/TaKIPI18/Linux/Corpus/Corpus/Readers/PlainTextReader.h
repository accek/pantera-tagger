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

#ifndef CORPUSLIB_PLAINTEXTREADER_H
#define CORPUSLIB_PLAINTEXTREADER_H

#include "../../corpuslib_headers.h"
#include "../Token.h"
#include "../Transducer.h"
#include "../Char.h"
#include "../CharUTF.h"
#include "../CharASCII.h"
#include "Reader.h"

namespace Corpus
{
	namespace Readers
	{
		class CORPUS_API CPlainTextReader: public CReader
		{
		public:
			/**
			Po zako�czeniu pracy z wszystkimi readerami (dowolnego typu) nale�y wywo�a� CReader::Clean()...
			*/
			CPlainTextReader(std::istream& stream, std::string grammarFileName);
			CPlainTextReader(std::istream& stream, CTransducer* transducer);
			~CPlainTextReader(void);

			CToken* GetToken();

		protected:
			std::queue<CToken*>* tokens;
			CTransducer* transducer;
			CCharUTF* ParseCharUTF(unsigned char prefix);
			CChar* ReadNextChar();

		private:
			bool externalTransducer;
		};
	}
}
#endif

