/*  Biblioteka JOSKIPI
 *  Jƒôzyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc≈Çawskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardy≈Ñski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_TESTOPER_H
#define LIBJOSKIPI_TESTOPER_H

#include "../joskipi_headers.h"
#include "Operator.h"
#include "Condition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Opakowuje testu logiczny w operator. <br>
		Atrybut position nie ma znaczenia w tej klasie, test juø jest umiejscowiony wzglÍdnie w ramach zdania, 
		wartoúÊ dowolna.
		*/
		class JOSKIPI_API CTestOper :
			public COperator
		{
		public:
			CTestOper(void);
			CTestOper(CCondition* condition);
			/**
			Obiekt zarzπdza obiektem testu, tzn. niszczy test.
			*/
			~CTestOper(void);

			/**
			Zwraca wartoúÊ zwrÛconπ przez test->Evaluate().
			*/
			bool GetValue(ValueVector& val, Corpus::CSentence& sentence);
			void GetValueNames(std::stringstream& out);

			std::string ToString();

		protected:
			/**
			Wskaünik na obiekt testu.
			*/
			CCondition* test;
		};
	}
}
#endif //LIBJOSKIPI_TESTOPER_H

