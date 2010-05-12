/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_LOOKLBINCC_H
#define LIBJOSKIPI_LOOKLBINCC_H

#include "../joskipi_headers.h"
#include "LookBinCC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Klasa operatora-warunku poszukuj�cego pozycji tokenu spe�niaj�cego podany warunek sk�adowy (conditions[0]). 
		Szuka od endPos w lewo do startPos.

		Je�eli go znajdzie to zostawia varPos ustawione na pozycj� odnalezionego elementu (w przeciwnym wypadku 
		zostawia warto�� CPosition::nowhere).
		*/
		class JOSKIPI_API CLookLeftBinCC: public CLookBinCC
		{
		public:
			/**
			Identyczny jak w klasie bazowej
			*/
			CLookLeftBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions);
			virtual ~CLookLeftBinCC(void);

			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}

}
#endif //LIBJOSKIPI_LOOKLBINCC_H

