/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_EQUALSC_H
#define LIBJOSKIPI_EQUALSC_H

#include "../joskipi_headers.h"
#include "SimpleCondition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Klasa binarnego testu r�wno�ci warto�ci reprezentowanych przez dwa obiekty CCondElem.
		*/
		class JOSKIPI_API CEqualSC : public CSimpleCondition
		{
		public:
			/**
			Konstruktor identyczny jak bazowy, rzuca wyj�tek, gdy przekazany wi�cej ni� jeden CCondElem.
			\throw std::string gdy cndElems.size() == 0
			\todo Powinien rzucac wyjatek r�wnie� gdy cndElems.size() 1 lub mniej
			*/
			CEqualSC(std::vector<CCondElem*>& cndElems) throw (std::string);
			virtual ~CEqualSC(void);

			/**
			1. Przekazane zdanie przekazuje dalej do metod GetValue obiekt�w z wektora. <br>
			2. cndElements i warto�ci s� zapisywane do wektor�w z tablicy valBufTab. 
			3. Por�wnuje warto�ci zapisane w wektorach w tablicy \valBufTab.
			4. if warto�ci parami identyczne (czyli po ich poszczeg�lnych elementach return true else return fals

			*/
			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}

}
#endif

