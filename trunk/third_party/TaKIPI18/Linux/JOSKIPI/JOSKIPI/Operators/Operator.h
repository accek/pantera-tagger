/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		Abstrakcyjny korzeñ ca³ej hierarchii klas. Wylicza wartoœæ na podstawie stanu
		wskazanych tokenów w drzewie. Wartoœæ (lub wartoœci w przypadku zbioru) jest
		zwracana poprzez ustawienie stanu wektora obiektów klasy CValue przekazanego
		przez referencjê w metodzie GetValue .

		Uwaga: obiekty wstawione do przekazanego wektora bêdzie niszczy³ w³aœciciel tego wektora!

		\todo do czego s³u¿y collectNames
		\todo do czego s³u¿y hadZeroDuringValuesGet
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
			Aktualna wzglêdna pozycja w zdaniu, liczona wzglêdem aktualnej pozycji ujednoznaczniania w zdaniu, 
			mo¿e przyjmowaæ wartoœci ujemne.
			*/
			pos_ptr position;
		};
	}
}

#endif

