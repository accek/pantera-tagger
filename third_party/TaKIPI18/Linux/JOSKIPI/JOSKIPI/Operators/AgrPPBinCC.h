/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_AGRPPBINCC_H
#define LIBJOSKIPI_AGRPPBINCC_H

#include "../joskipi_headers.h"
#include "BinaryComplexC.h"
#include "ConstValue.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		\todo Opis CAgrPPBinCC
		*/
		class JOSKIPI_API CAgrPPBinCC:
			public CBinaryComplexC
		{
		public:
			CAgrPPBinCC(std::vector<pos_ptr>* positions, CConstValue* atrSet, int bits);
			virtual ~CAgrPPBinCC(void);

			std::string ToString();

		protected:
			CConstValue* attributes;

			virtual bool Evaluate(Corpus::CSentence& sentence);

			// Liczba bit�w kt�ra musi by� zachowana po maskowaniu.
			int bits;
		};
	}

}
#endif //LIBJOSKIPI_AGRPPBINCC_H

