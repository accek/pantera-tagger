/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_ISBIG_H
#define LIBJOSKIPI_ISBIG_H

#include "../joskipi_headers.h"
#include "SimpleCondition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Unarny warunek sprawdzaj¹cy czy wartoœæ (koniecznie typu string) ma du¿¹ literê na pocz¹tku.
		*/
		class JOSKIPI_API CIsBigSC :
			public CSimpleCondition
		{
		public:
			/**
			\throw std::string gdy cndElems.size() != 1
			*/
			CIsBigSC(std::vector<CCondElem*>& cndElems) throw (std::string);
			virtual ~CIsBigSC(void);

			/**
			Zwraca true, gdy wartoœæ dostarczona przez cndElements jest stringiem z pierwsz¹ du¿¹ liter¹.
			*/
			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}

}
#endif

