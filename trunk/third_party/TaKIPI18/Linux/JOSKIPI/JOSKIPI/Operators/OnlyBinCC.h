/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		Warunek sprawdzaj¹cy, czy pomiêdzy podanymi dwiema pozycjami wszystkie tokeny spe³niaj¹ podany warunek.
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
