/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBKJOSKIPI_IN_H
#define LIBKJOSKIPI_IN_H

#include "../joskipi_headers.h"
#include "SimpleCondition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Klasa binarnego testu zawierania si� warto�ci/zbioru pierwszego argumentu (tj. cndElements[0]) w warto�ci drugiego.
		*/
		class JOSKIPI_API CInSC :
			public CSimpleCondition
		{
		public:
			/**
			\throw std::string gdy cndElems.size() != 2
			*/
			CInSC(std::vector<CCondElem*>& cndElems) throw(std::string);
			virtual ~CInSC(void);

			/**
			1. Przekazane zdanie przekazuje dalej do metod GetValue obiekt�w z wektora cndElements i warto�ci s� zapisywane 
			do obiekt�w z tablicy valBufTab. <br>
			2. if tylko jedna warto�� w valBufTab[0] i valBufTab[0] (por�wnujemy warto�ci wyra�one na jednym int, 
			ale ci�gle mog� to by� sumy bitowej)<br>
			(a) if wszystkie bity pierwszej warto�ci zawieraj� si� w drugiej lub warto�ci s� identyczne 
			(np. w przypadku string�w) return true else return false<br>
			3 else<br>
			(a) if wszystkie warto�ci z valBufTab[0] znajduj� si� w valBufTab[1] return true else return false.
			*/
			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}
}
#endif

