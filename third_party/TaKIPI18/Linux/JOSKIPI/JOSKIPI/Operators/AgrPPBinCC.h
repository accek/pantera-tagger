/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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

			// Liczba bitów która musi byæ zachowana po maskowaniu.
			int bits;
		};
	}

}
#endif //LIBJOSKIPI_AGRPPBINCC_H

