/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_LOOKRBINCC_H
#define LIBJOSKIPI_LOOKRBINCC_H

#include "../joskipi_headers.h"
#include "LookBinCC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Klasa operatora-warunku poszukuj�cego pozycji tokenu spe�niaj�cego podany warunek sk�adowy (conditions[0]). 
		Szuka od startPos w prawo do endPos wykorzystuj�c varPos do iteracji i zwr�cenia odnalezionej pozycji.

		Je�eli go znajdzie to ustawia varPos na pozycj� odnalezionego elementu (w przeciwnym razie CPosition::nowhere).
		*/
		class JOSKIPI_API CLookRightBinCC: public CLookBinCC
		{
		public:
			/**
			Identyczny jak w klasie bazowej
			*/
			CLookRightBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions);
			virtual ~CLookRightBinCC(void);

			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}
}
#endif

