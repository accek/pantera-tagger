/*  Biblioteka Corpus
 *  Zestaw narzÄ™dzi do przetwarzania tekstÃ³w w jÄ™zyku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef CORPUSLIB_TAGGEDDOCUMENT_H
#define CORPUSLIB_TAGGEDDOCUMENT_H

#include "../../corpuslib_headers.h"
#include "Document.h"
#include "../Readers/CorporaReader.h"

namespace Corpus
{
	namespace Documents
	{
		class CORPUS_API TaggedDocument:
			public Document
		{
		public:
			///Po zakoñczeniu pracy z wszystkimi dokumentami i readerami (dowolnego typu) nale¿y wywo³aæ CReader::Clean()...
			TaggedDocument(std::string filename, bool dissambOnly = false);
			virtual ~TaggedDocument(void);
			/**
			Zwraca informacje, czy dany dokument, jest dokumentem otagowanym.
			*/
			virtual inline bool IsTagged() {return true;}
			/**
			Zwraca informacje, czy dany dokument, jest dokumentem sparsowanym.
			*/
			virtual inline bool IsParsed(){return false;}
			/**
			Otwiera plik dokumentu do czytania (tworzy obiekt Reader'a).
			*/
			virtual void Open() throw(std::string);
			/**
			Zwraca nastêpny wyraz dokumentu.
			\return Obiekty $Wyraz$ lub null, gdy nie mo¿na odczytaæ wyrazu lub koniec pliku.
			*/
			virtual CToken* NextToken();
			/**
			Zwraca nastêpne zdanie z dokumentu.
			\return Obiekty $Zdanie$ lub null, gdy nie mo¿na odczytaæ wyrazu lub koniec pliku.
			*/
			virtual CSentence* NextSentence();

		private:
			bool m_bDissambOnly;
		};
	}
}

#endif
