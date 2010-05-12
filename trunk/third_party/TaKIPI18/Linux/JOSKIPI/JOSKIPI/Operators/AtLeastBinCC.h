/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_ATLEASTBINCC_H
#define LIBJOSKIPI_ATLEASTBINCC_H

#include "../joskipi_headers.h"
#include "BinaryComplexC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Operator poszukuj¹cy okreœlonej liczby tokenów spe³niaj¹cych warunek condtions[0], które ma odnaleŸæ pomiêdzy 
		startPos i endPos. W razie, gdyby nie znalaz³ zwraca false.
		*/
		class JOSKIPI_API CAtLeastBinCC :
			public CBinaryComplexC
		{
		public:
			/**
			Identyczny jak w klasie bazowej, dodatkowo tylko pobiera iloœæ tokenów.
			*/
			CAtLeastBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions, int num);
			virtual ~CAtLeastBinCC(void);

			virtual bool Evaluate(Corpus::CSentence& sentence);

			/**
			\todo Implementacja CAtLeastBinCC::ToString()
			*/
			virtual std::string ToString();

		protected:
			/**
			Minimalna liczba tokenów zgodnych z warunkiem, które ma znaleŸæ (mo¿e ich byæ wiêcej).
			*/
			int num;
		};
	}

}
#endif // LIBJOSKIPI_ATLEASTBINCC_H

