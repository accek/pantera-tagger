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

#ifndef CORPUSLIB_TEXTDOCUMENT_H
#define CORPUSLIB_TEXTDOCUMENT_H

#include "../../corpuslib_headers.h"
#include "Document.h"
#include "../Transducer.h"
#include "../Readers/PlainTextReader.h"

namespace Corpus
{
	namespace Documents
	{
		/**
		Klasa reprezentuje prosty dokument w postaci pliku tekstowego, lub pliku XML.
		*/
		class CORPUS_API TextDocument :
			public Document
		{
		public:
			/**
			Konstruktor
			Po zako�czeniu pracy z wszystkimi dokumentami i readerami (dowolnego typu) nale�y wywo�a� CReader::Clean()...
			\param nazwaPliku Nazwa pliku dokumentu.
			\param czyXml Informacja czy dokument jest plikiem XML (je�li nie zak�ada si�, �e jest plikuem tekstowym).
			*/
			TextDocument(std::string filename, bool xml = false);
			/**
			Destruktor
			*/
			~TextDocument(void);
			/**
			Zwraca informacj� czy dokument jest dokumentem w postaci XML'a.
			\returns Czy dokument jest w postaci XML'a.
			*/
			inline bool IsXml() { return isXml;}
			/**
			Zwraca informacj� czy dokument jest dokumentem w postaci pliku tekstowego.
			\returns Czy dokument jest w postaci pliku tekstowego.
			*/
			inline bool IsTxt() { return !isXml; }
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

		private:
			/**
			Czy podany plik jest w postaci XML'a.
			*/
			bool isXml;
			/**
			(statyczny) Transduktor gramatyki zadawanej z pliku.
			*/
			static CTransducer* transducer;
		};
	}
}
#endif
