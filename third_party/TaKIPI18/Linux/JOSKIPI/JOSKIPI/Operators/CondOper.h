/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
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
		Obiekt tej klasy opakowuje inny obiekt COperator w dost�p warunkowany
		wynikiem testu. Operator zwracaj�cy warto�� nieokre�lon� (brak warto�ci �
		kodowany w drzewie '_'), gdy test daje w wyniku sprawdzenia warto�� false
		*/
		class JOSKIPI_API CCondOper: public COperator
		{
		public:
			CCondOper(void);
			/**
			Dostaje utworzone ju� obiekty w postaci wska�nik�w, kt�re zapami�tuje. Zostan� one jednak skasowane w destruktorze.
			*/
			CCondOper(CCondition* condition, COperator* op) throw(std::string);
			CCondOper(const CCondOper& op);
			/**
			Obiekt zarz�dza wszystkimi obiektami, do kt�rych ma wska�niki, tzn. niszczy underCond i test .
			*/
			~CCondOper(void);

			const CCondOper& operator=(const CCondOper& op);

			/**
			Dla podanego zdania sprawdza warto�� log. warunku test->Evaluate().
			if warto�� true to zwraca underCond->GetValue else warto�� pust�.
			*/
			bool GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence);
			void GetValueNames(std::stringstream& out);

			std::string ToString();

		protected:
			/**
			Wska�nik na obiekt testu
			*/
			CCondition* test;
			/**
			Wska�nik na warunkowany obiekt COperator
			*/
			COperator* underCond;
		};
	}
}

#endif //LIBJOSKIPI_CONDOPER_H

