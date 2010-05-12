/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		Klasa binarnego testu równoœci wartoœci reprezentowanych przez dwa obiekty CCondElem.
		*/
		class JOSKIPI_API CEqualSC : public CSimpleCondition
		{
		public:
			/**
			Konstruktor identyczny jak bazowy, rzuca wyj¹tek, gdy przekazany wiêcej ni¿ jeden CCondElem.
			\throw std::string gdy cndElems.size() == 0
			\todo Powinien rzucac wyjatek równie¿ gdy cndElems.size() 1 lub mniej
			*/
			CEqualSC(std::vector<CCondElem*>& cndElems) throw (std::string);
			virtual ~CEqualSC(void);

			/**
			1. Przekazane zdanie przekazuje dalej do metod GetValue obiektów z wektora. <br>
			2. cndElements i wartoœci s¹ zapisywane do wektorów z tablicy valBufTab. 
			3. Porównuje wartoœci zapisane w wektorach w tablicy \valBufTab.
			4. if wartoœci parami identyczne (czyli po ich poszczególnych elementach return true else return fals

			*/
			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}

}
#endif

