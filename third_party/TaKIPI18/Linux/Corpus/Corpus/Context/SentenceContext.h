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

#ifndef CORPUSLIB_SENTENCECONTEXT_H
#define CORPUSLIB_SENTENCECONTEXT_H

#include "../../corpuslib_headers.h"
#include "Context.h"

namespace Corpus
{
	namespace Context
	{
		/**
		Klasa reprezentuj¹ca kontekst zdaniowy.
		*/
		class CORPUS_API SentenceContext: 	public Context
		{
		public:
			SentenceContext(Corpus::Documents::Document* doc);
			~SentenceContext();
			/**
			Przesuwa kontekst na pierwsz¹ pozycjê wed³ug klasy kontekstu.
			*/
			virtual bool FirstPosition();
			/**
			Przesuwa kontekst w przód weg³ug kryterium klasy kontekstu.
			*/
			virtual bool NextPosition();
			/**
			Operator do czytania z kontekstu zdaniowego.
			\param n zwraca n'ty wyraz w zdaniu, ustawia obecny wyraz na danej pozycji
			\return Wyraz z kontekstu.
			*/
			virtual CToken* operator[](int n);
			/**
			Mówi gdzie jest œrodek kontekstu.
			*/
			virtual	int GetPosition()
			{
				return m_sentence->Size()/2;;
			}
			/**
			Zwraca rozmiar kontekstu.
			*/
			virtual inline int Size()
			{
				return m_sentence->Size();
			}

			/**
			\return Obiekt CSentence utworzony na podstawie bie¿¹cego zdania.
			*/
			CSentence* GetCurrentSentence()
			{
				return m_sentence;
			}
		private:
			CSentence *m_sentence;
			int m_iSentence;
		};
	}
}

#endif
