/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_INTERSEC_H
#define LIBJOSKIPI_INTERSEC_H

#include "../joskipi_headers.h"
#include "SimpleCondition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Klasa binarnego testu cz�ci wsp�lnej warto�ci/zbioru pierwszego argumentu (tj. cndElements[0]) z warto�ci� drugiego 
		(dla pojedynczych warto�ci binarnych oznacza to wyst�powanie wsp�lnych bit�w).
		*/
		class JOSKIPI_API CIntersecSC :
			public CSimpleCondition
		{
		public:
			/**
			\throw std::string gdy cndElems.size() != 2
			*/
			CIntersecSC(std::vector<CCondElem*>& cndElems) throw (std::string);
			virtual ~CIntersecSC(void);

			/**

			1. Przekazane zdanie przekazuje dalej do metod GetValue obiekt�w z wektora cndElements i warto�ci s� 
			zapisywane do obiekt�w z tablicy valBufTab. <br>
			2. if tylko jedna warto�� w valBufTab[0] i valBufTab[0] (por�wnujemy warto�ci wyra�one na jednym int, ale ci�gle 
			mog� to by� sumy bitowej)<br>
			(a) if istniej� wsp�lne bity pierwszej i drugiej warto�ci lub dwie warto�ci stringowe s� identyczne 
			return true else return false <br>
			2. else <br>
			(a)if istniej� warto�ci z valBufTab[0], kt�re znajduj� si� w valBufTab[1] return true else return false.<br>
			*/
			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}
}
#endif // LIBJOSKIPI_INTERSEC_H

