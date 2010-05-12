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

#ifndef CORPUSLIB_WINDOWCONTEXT_H
#define CORPUSLIB_WINDOWCONTEXT_H

#include "../../corpuslib_headers.h"
#include "Context.h"

namespace Corpus
{
	namespace Context
	{
		/**
		Klasa reprezentuj¹ca kontekst oknowy.
		*/
		class CORPUS_API WindowContext:
			public Context
		{
		public:
			WindowContext(Corpus::Documents::Document* doc, int nBefore, int nAfter);
			~WindowContext();
			/**
			Przesuwa kontekst na pierwsz¹ pozycjê wed³ug klasy kontekstu.
			*/
			virtual bool FirstPosition();
			/**
			Przesuwa kontekst w przód weg³ug kryterium klasy kontekstu.
			*/
			virtual bool NextPosition();
			/**
			Operator do czytania kontekstu.
			\param ktory Który wyraz w kontekscie (0 - pozycja obecja, 1 nastêpny wyraz, -1 poprzedni wyraz, +2 drugi wyraz po, itd...).
			\return Wyraz z kontekstu.
			*/
			virtual CToken* operator[](int n);
			/**
			Mówi gdzie jest œrodek kontekstu.
			*/
			virtual	int GetPosition()
			{
				return (int)tokensBefore.size();
			}
			/**
			Zwraca rozmiar kontekstu.
			*/
			virtual inline int Size()
			{
				if(!currentToken)
					return 0;
				else
					return (int)tokensBefore.size() + 1 + (int)tokensAfter.size();
			}

		private:
			CToken* currentToken;
			int before;
			int after;
			std::vector<CToken*> tokensBefore;
			std::vector<CToken*> tokensAfter;
		};
	}
}

#endif
