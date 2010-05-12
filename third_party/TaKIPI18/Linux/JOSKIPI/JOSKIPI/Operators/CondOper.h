/*  Biblioteka JOSKIPI
 *  Jƒôzyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc≈Çawskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardy≈Ñski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_CONDOPER_H
#define LIBJOSKIPI_CONDOPER_H

#include "../joskipi_headers.h"
#include "Operator.h"
#include "Condition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Obiekt tej klasy opakowuje inny obiekt COperator w dostÍp warunkowany
		wynikiem testu. Operator zwracajπcy wartoúÊ nieokreúlonπ (brak wartoúci ó
		kodowany w drzewie '_'), gdy test daje w wyniku sprawdzenia wartoúÊ false
		*/
		class JOSKIPI_API CCondOper: public COperator
		{
		public:
			CCondOper(void);
			/**
			Dostaje utworzone juø obiekty w postaci wskaünikÛw, ktÛre zapamiÍtuje. Zostanπ one jednak skasowane w destruktorze.
			*/
			CCondOper(CCondition* condition, COperator* op) throw(std::string);
			CCondOper(const CCondOper& op);
			/**
			Obiekt zarzπdza wszystkimi obiektami, do ktÛrych ma wskaüniki, tzn. niszczy underCond i test .
			*/
			~CCondOper(void);

			const CCondOper& operator=(const CCondOper& op);

			/**
			Dla podanego zdania sprawdza wartoúÊ log. warunku test->Evaluate().
			if wartoúÊ true to zwraca underCond->GetValue else wartoúÊ pustπ.
			*/
			bool GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence);
			void GetValueNames(std::stringstream& out);

			std::string ToString();

		protected:
			/**
			Wskaünik na obiekt testu
			*/
			CCondition* test;
			/**
			Wskaünik na warunkowany obiekt COperator
			*/
			COperator* underCond;
		};
	}
}

#endif //LIBJOSKIPI_CONDOPER_H

