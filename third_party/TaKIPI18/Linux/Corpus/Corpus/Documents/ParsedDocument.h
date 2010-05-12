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

#ifndef CORPUS_PARSEDDOCUMENT_H
#define CORPUS_PARSEDDOCUMENT_H

#include "../../corpuslib_headers.h"
#include "Document.h"

namespace Corpus
{
	namespace Documents
	{
		/**
		TODO: Dorobi� t�
		*/
		class CORPUS_API ParsedDocument :
			public Document
		{
		public:
			///Po zako�czeniu pracy z wszystkimi dokumentami i readerami (dowolnego typu) nale�y wywo�a� CReader::Clean()...
			ParsedDocument(std::string filename);
			~ParsedDocument(void);
			/**
			Zwraca informacje, czy dany dokument, jest dokumentem otagowanym.
			*/
			virtual inline bool IsTaggeds() {return false;}
			/**
			Zwraca informacje, czy dany dokument, jest dokumentem sparsowanym.
			*/
			virtual inline bool IsParsed(){return true;}
			/**
			Otwiera plik dokumentu do czytania (tworzy obiekt Reader'a).
			*/
			virtual void Open() throw(std::string);
			/**
			Zwraca nast�pny wyraz dokumentu.
			\return Obiekty $Wyraz$ lub null, gdy nie mo�na odczyta� wyrazu lub koniec pliku.
			*/
			virtual CToken* NextToken();
			/**
			Zwraca nast�pne zdanie z dokumentu.
			\return Obiekty $Zdanie$ lub null, gdy nie mo�na odczyta� wyrazu lub koniec pliku.
			*/
			virtual CSentence* NextSentence();
		};
	}
}
#endif
