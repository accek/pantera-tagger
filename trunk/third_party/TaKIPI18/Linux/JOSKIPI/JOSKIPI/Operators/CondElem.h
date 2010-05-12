/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		Klasa abstrakcyjna generalizuj¹ca elementy warunku (condition).
		Reprezentuje wartoœæ wystêpuj¹c¹ w warunkach (pochodne po CCondition ).
		Wartoœæ ta mo¿e byæ sta³a ( CConstValue ) lub wyliczana na podstawie stanu
		tokenów ( COperator ).
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

