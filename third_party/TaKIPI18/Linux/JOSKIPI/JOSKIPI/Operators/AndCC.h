/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_AND_H
#define LIBJOSKIPI_AND_H

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
		class JOSKIPI_API CAndCC: public CComplexCond
		{
		public:
			CAndCC(std::vector<CCondition*>* conditions);
			virtual ~CAndCC(void);

			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}
}
#endif // LIBJOSKIPI_AND_H

