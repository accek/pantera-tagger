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

#ifndef CORPUSLIB_CONTEXT_H
#define CORPUSLIB_CONTEXT_H

#include "../../corpuslib_headers.h"
#include "../Documents/Document.h"
#include "../Token.h"

namespace Corpus
{
	namespace Context
	{
		/**
		(Abstrakcyjna) Klasa reprezentuj¹ca kontekst wyrazu, jest to kontekst przesuwalny (przesuwalne okno na dokumencie).
		*/
		class CORPUS_API Context
		{
		protected:
			Context(Corpus::Documents::Document* doc);

		public:
			virtual ~Context();
			/**
			Przesuwa kontekst na pierwsz¹ pozycjê wed³ug klasy kontekstu.
			*/
			virtual bool FirstPosition() = 0;
			/**
			Przesuwa kontekst w przód weg³ug kryterium klasy kontekstu.
			*/
			virtual bool NextPosition() = 0;
	//		/**
	//		Przesuwa kontekst na piewsz¹ pozycjê weg³ug kryterium klasy kontekstu, po elementach przefiltrowanych.
	//		*/
	//		virtual bool PierwszaPozycjaFiltrowana() = 0;
	//		/**
	//		Przesuwa kontekst w przód weg³ug kryterium klasy kontekstu, po elementach przefiltrowanych.
	//		*/
	//		virtual bool NastepnaPozycjaFiltrowana() = 0;
			/**
			Operator do czytania kontekstu.
			\param n Który wyraz w kontekscie (0 - pozycja obecja, 1 nastêpny wyraz, -1 poprzedni wyraz, +2 drugi wyraz po, itd...).
			\return Wyraz z kontekstu.
			*/
			virtual CToken* operator[](int n) = 0;
			/**
			Zwraca dokument kontekstu.
			\return Dokument zwi¹zany z kontekstem.
			*/
			inline Corpus::Documents::Document* GetDocument() { return document; }
			/**
			Mówi gdzie jest œrodek kontekstu.
			*/
			virtual int GetPosition() = 0;
			/**
			Zwraca rozmiar kontekstu.
			*/
			virtual inline int Size() = 0;


		protected:
			Corpus::Documents::Document* document;
		};
	}
}
#endif
