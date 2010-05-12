/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_ONLYBINCC_H
#define LIBJOSKIPI_ONLYBINCC_H

#include "../joskipi_headers.h"
#include "BinaryComplexC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Warunek sprawdzaj�cy, czy pomi�dzy podanymi dwiema pozycjami wszystkie tokeny spe�niaj� podany warunek.
		*/
		class  JOSKIPI_API COnlyBinCC: public CBinaryComplexC
		{
		public:
			/**
			Identyczny jak w klasie bazowej
			*/
			COnlyBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions);
			virtual ~COnlyBinCC(void);

			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}
}
#endif
