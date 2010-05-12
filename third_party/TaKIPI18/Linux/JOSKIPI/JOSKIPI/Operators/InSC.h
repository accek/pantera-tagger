/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		Klasa binarnego testu zawierania siê wartoœci/zbioru pierwszego argumentu (tj. cndElements[0]) w wartoœci drugiego.
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
			1. Przekazane zdanie przekazuje dalej do metod GetValue obiektów z wektora cndElements i wartoœci s¹ zapisywane 
			do obiektów z tablicy valBufTab. <br>
			2. if tylko jedna wartoœæ w valBufTab[0] i valBufTab[0] (porównujemy wartoœci wyra¿one na jednym int, 
			ale ci¹gle mog¹ to byæ sumy bitowej)<br>
			(a) if wszystkie bity pierwszej wartoœci zawieraj¹ siê w drugiej lub wartoœci s¹ identyczne 
			(np. w przypadku stringów) return true else return false<br>
			3 else<br>
			(a) if wszystkie wartoœci z valBufTab[0] znajduj¹ siê w valBufTab[1] return true else return false.
			*/
			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}
}
#endif

