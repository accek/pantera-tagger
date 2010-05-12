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

#ifndef CORPUSLIB_DOCUMENTCONTEXT_H
#define CORPUSLIB_DOCUMENTCONTEXT_H

#include "../../corpuslib_headers.h"
#include "Context.h"

namespace Corpus
{
	namespace Context
	{
		class CORPUS_API DocumentContext :	public Context
		{
		public:
			DocumentContext(Corpus::Documents::Document* doc);
			~DocumentContext(void);
			/**
			Przesuwa kontekst na pierwsz� pozycj� wed�ug klasy kontekstu.
			*/
			virtual bool FirstPosition() { position = 0; return true; }
			/**
			Przesuwa kontekst w prz�d weg�ug kryterium klasy kontekstu.
			*/
			virtual bool NextPosition()
			{
				position++;
				if(position== (int)tokens.size())
					return false;
				else
					return true;
			}
			/**
			Operator do czytania kontekstu.
			\param n Kt�ry wyraz w kontekscie [0;Rozmiar()-1]
			\return Wyraz z kontekstu.
			*/
			virtual CToken* operator[](int n);
			/**
			Podaje rozmiar danego dokumentu, wyra�ony w ilo�ci wyraz�w.
			*/
			virtual inline int Size() { return (int)tokens.size(); }
			/**
			M�wi gdzie jest �rodek kontekstu.
			*/
			virtual	int GetPosition()
			{
				return position;
			}

		private:
#ifdef WIN32
			template class CORPUS_API std::allocator<CToken*>;
			template class CORPUS_API std::vector<CToken*, std::allocator<CToken*> >;
#endif
			std::vector<CToken*> tokens;
			int position;
		};
	}
}

#endif
