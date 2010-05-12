/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIP_NOT_H
#define LIBJOSKIP_NOT_H

#include "../joskipi_headers.h"
#include "ComplexCond.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Klasy implementuj¹ce podstawowe operacje logiczne na pod³¹czonej liœcie warunków. 
		Interpretacja jest jednoznaczna z wyj¹tkiem CNotCC dla wiêcej ni¿ jednego warunku. 
		W tym ostatnim przypadku zak³adamy, ¿e jest negacja z dysjunkcji warunków, 
		tzn. odpowiada to CNotCC(..., COrCC(...)).
		*/
		class JOSKIPI_API CNotCC: public CComplexCond
		{
		public:
			CNotCC(std::vector<CCondition*>* conditions);
			virtual ~CNotCC(void);

			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}

}
#endif // LIBJOSKIP_NOT_H

