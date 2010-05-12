/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
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
		Atrybut position nie ma znaczenia w tej klasie, test ju� jest umiejscowiony wzgl�dnie w ramach zdania, 
		warto�� dowolna.
		*/
		class JOSKIPI_API CTestOper :
			public COperator
		{
		public:
			CTestOper(void);
			CTestOper(CCondition* condition);
			/**
			Obiekt zarz�dza obiektem testu, tzn. niszczy test.
			*/
			~CTestOper(void);

			/**
			Zwraca warto�� zwr�con� przez test->Evaluate().
			*/
			bool GetValue(ValueVector& val, Corpus::CSentence& sentence);
			void GetValueNames(std::stringstream& out);

			std::string ToString();

		protected:
			/**
			Wska�nik na obiekt testu.
			*/
			CCondition* test;
		};
	}
}
#endif //LIBJOSKIPI_TESTOPER_H

