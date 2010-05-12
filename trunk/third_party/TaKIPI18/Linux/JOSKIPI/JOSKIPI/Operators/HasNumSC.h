/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_HASNUMOP_H
#define LIBJOSKIPI_HASNUMOP_H

#include "../joskipi_headers.h"
#include "SimpleCondition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		class JOSKIPI_API CHasNumSC :
			public CSimpleCondition
		{
		public:
			CHasNumSC(std::vector<CCondElem*>& cndElems);
			virtual ~CHasNumSC(void);

			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}
}
#endif

