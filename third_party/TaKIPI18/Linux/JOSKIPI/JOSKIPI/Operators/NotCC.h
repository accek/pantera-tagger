/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
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
		Klasy implementuj�ce podstawowe operacje logiczne na pod��czonej li�cie warunk�w. 
		Interpretacja jest jednoznaczna z wyj�tkiem CNotCC dla wi�cej ni� jednego warunku. 
		W tym ostatnim przypadku zak�adamy, �e jest negacja z dysjunkcji warunk�w, 
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

