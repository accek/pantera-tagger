/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_SIMPLEOPERATOR_H
#define LIBJOSKIPI_SIMPLEOPERATOR_H

#include "../joskipi_headers.h"
#include "Condition.h"
#include "CondElem.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Abstrakcyjna klasa bazowa dla klas prostych testów logicznych porównuj¹cych ze sob¹ dwa operatory 
		lub operator z wartoœci¹ sta³¹ (pojedyncza lub zbiór).

		<br>
		\todo Powinien zawieraæ atrybut: valBufTab --- prosty bufor-tablica na wartoœci pobierane z obiektów CCondElem podczas dzia³ania, 
		length(valBufTab)=length(cndElements) --- poprawa efektywnoœci, aby nie tworzyæ za ka¿dym razem od nowa. Istnieje zmienna
		valBufTab w CEqualSC::Evaluate (i innych pochodnych po CSimpleCondition) -> nie jako tablica ale jako std::vector

		*/
		class JOSKIPI_API CSimpleCondition :
			public CCondition
		{
		public:
			/**
			Dostaje wektor obiektów klasy CCondElem --- czyli operatorów i wartoœci. 
			Wszystkie obiekty w postaci wskaŸników --- zarz¹dza nimi. 
			Tablicê positions ustawia na pust¹ --- wszystkie pozycje w zdaniu zdefiniowane s¹ przez obiekty CCondElem.
			*/
			CSimpleCondition(std::vector<CCondElem*>& cndElems);
			/**
			\todo Obiekt zarz¹dza obiektem buforu, tzn. niszczy valBufTab. 
			*/
			virtual ~CSimpleCondition(void);

		protected:
			/**
			cndElements --- wektor elementów na których dzia³a dany warunek (na razie bêd¹ tylko dwa, 
			ale w przysz³oœci moga te¿ byæ operatory 3-arg.).
			*/
			std::vector<CCondElem*>& cndElems;
		};
	}
}
#endif //LIBJOSKIPI_SIMPLEOPERATOR_H

