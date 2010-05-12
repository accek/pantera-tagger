/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_OPERATOR_H
#define LIBJOSKIPI_OPERATOR_H

#include "../joskipi_headers.h"
#include "CondElem.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Abstrakcyjny korze� ca�ej hierarchii klas. Wylicza warto�� na podstawie stanu
		wskazanych token�w w drzewie. Warto�� (lub warto�ci w przypadku zbioru) jest
		zwracana poprzez ustawienie stanu wektora obiekt�w klasy CValue przekazanego
		przez referencj� w metodzie GetValue .

		Uwaga: obiekty wstawione do przekazanego wektora b�dzie niszczy� w�a�ciciel tego wektora!

		\todo do czego s�u�y collectNames
		\todo do czego s�u�y hadZeroDuringValuesGet
		\todo na diagramie ArgoUML nie widac relacji COperator->CPosition
		*/
		class JOSKIPI_API COperator :
			public CCondElem
		{
		public:
			COperator();
			COperator(pos_ptr position);
			COperator(const COperator& op);
			virtual ~COperator(void);

			// Operator przypisania.
			const COperator& operator=(const COperator& op);

			virtual void GetValueNames(std::stringstream& out) = 0;

			static bool collectNames;
			friend class CCondOper;

			virtual std::string ToString()
			{
				return "COperator...";
			}

		protected:
			bool hadZeroDuringValuesGet;
			/**
			Aktualna wzgl�dna pozycja w zdaniu, liczona wzgl�dem aktualnej pozycji ujednoznaczniania w zdaniu, 
			mo�e przyjmowa� warto�ci ujemne.
			*/
			pos_ptr position;
		};
	}
}

#endif

