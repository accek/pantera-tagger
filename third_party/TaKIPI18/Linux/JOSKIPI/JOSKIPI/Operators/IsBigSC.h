/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
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
		Unarny warunek sprawdzaj�cy czy warto�� (koniecznie typu string) ma du�� liter� na pocz�tku.
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
			Zwraca true, gdy warto�� dostarczona przez cndElements jest stringiem z pierwsz� du�� liter�.
			*/
			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}

}
#endif

