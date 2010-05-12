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

#ifndef CORPUSLIB_DOCUMENT_H
#define CORPUSLIB_DOCUMENT_H

#include "../../corpuslib_headers.h"
#include "../Token.h"
#include "../Sentence.h"
#include "../Sentencer.h"

namespace Corpus
{
	namespace Documents
	{
		/**
		(Abstrakcyjna) Reprezentuje dokument wraz z informacjami o dokumencie.
		*/
		class CORPUS_API Document
		{
		public:
			/**
			Destruktor.

			Po zakoñczeniu pracy z wszystkimi dokumentami i readerami (dowolnego typu) nale¿y wywo³aæ CReader::Clean()...
			*/
			virtual ~Document(void);
			/**
			Zwraca informacje, czy dany dokument, jest dokumentem otagowanym.
			*/
			virtual inline bool IsTagged() {return false;}
			/**
			Zwraca informacje, czy dany dokument, jest dokumentem sparsowanym.
			*/
			virtual inline bool IsParsed(){return false;}
			/**
			Zwraca nazwê dokumentu.
			*/
			virtual inline std::string GetFilename() { return filename; }
			/**
			Otwiera plik dokumentu do czytania (tworzy obiekt Reader'a).
			*/
			virtual void Open() throw(std::string) = 0;
			/**
			Zamyka plik dokumentu (usuwa obiekt Reader'a).
			*/
			virtual void Close();
			/**
			Zwraca nastêpny wyraz dokumentu.
			\return Obiekty $Wyraz$ lub null, gdy nie mo¿na odczytaæ wyrazu lub koniec pliku.
			*/
			virtual CToken* NextToken() = 0;
			/**
			Zwraca nastêpne zdanie z dokumentu.
			\return Obiekty $Zdanie$ lub null, gdy nie mo¿na odczytaæ wyrazu lub koniec pliku.
			*/
			virtual CSentence* NextSentence() = 0;
			/**
			Ustawia siê na pocz¹tku dokumentu czytanego.
			*/
			void GotoStart()
			{
				Close();
				Open();
			}

			enum DocumentType
			{
				text  = 0,
				xmlText = 1,
				tagged  = 2,
				parsed = 3
			};

		protected:
			/**
			Kontruktor dostêpny tylko dla klas dziedzicz¹cych.
			\param nazwaPliku Nazwa pliku dokumentu.
			*/
			Document(std::string filename);

			/**
			Nazwa pliku dokumentu.
			*/
			std::string filename;
			/**
			Reader u¿ywany do czytania dokumentu
			*/
			Corpus::Readers::CReader* reader;
			/**
			Strumieñ plikowy.
			*/
			std::ifstream* file;
			/**
			Sentencer do tworzenia zdañ.
			*/
			CSentencer* sentencer_;
		};
	}
}

#endif
