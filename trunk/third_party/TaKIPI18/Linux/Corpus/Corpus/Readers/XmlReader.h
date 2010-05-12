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

#ifndef CORPUSLIB_XMLREADER_H
#define CORPUSLIB_XMLREADER_H

#include "../../corpuslib_headers.h"
#include "Reader.h"
#include "PlainTextReader.h"
#include "../Transducer.h"

namespace Corpus
{
	namespace Readers
	{

		class  CORPUS_API CXmlReader :
			public CReader
		{
		public:
			CXmlReader(std::istream& stream, std::string grammarFileName);
			~CXmlReader(void);
			CToken* GetToken();
	
		protected:
			CPlainTextReader* reader_;
			std::stringstream xmlTextBuffor_;
			CTransducer* transducer_;
			bool isChunkS_;
		};

	}
}
#endif

