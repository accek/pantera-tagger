/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_CONDELEM_H
#define LIBJOSKIPI_CONDELEM_H
#include "../joskipi_headers.h"

namespace JOSKIPI
{
	namespace Operators
	{
		typedef std::vector<Corpus::CValue > ValueVector;
		/**
		Klasa abstrakcyjna generalizuj�ca elementy warunku (condition).
		Reprezentuje warto�� wyst�puj�c� w warunkach (pochodne po CCondition ).
		Warto�� ta mo�e by� sta�a ( CConstValue ) lub wyliczana na podstawie stanu
		token�w ( COperator ).
		*/
		class JOSKIPI_API CCondElem
		{
		public:
			CCondElem(void);
			virtual ~CCondElem(void);

			virtual bool GetValue(ValueVector& val, Corpus::CSentence& sentence) = 0;

			virtual std::string ToString() = 0;
		};
	}

}
#endif //LIBJOSKIPI_CONDELEM_H

