/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
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
		Abstrakcyjna klasa bazowa dla klas prostych test�w logicznych por�wnuj�cych ze sob� dwa operatory 
		lub operator z warto�ci� sta�� (pojedyncza lub zbi�r).

		<br>
		\todo Powinien zawiera� atrybut: valBufTab --- prosty bufor-tablica na warto�ci pobierane z obiekt�w CCondElem podczas dzia�ania, 
		length(valBufTab)=length(cndElements) --- poprawa efektywno�ci, aby nie tworzy� za ka�dym razem od nowa. Istnieje zmienna
		valBufTab w CEqualSC::Evaluate (i innych pochodnych po CSimpleCondition) -> nie jako tablica ale jako std::vector

		*/
		class JOSKIPI_API CSimpleCondition :
			public CCondition
		{
		public:
			/**
			Dostaje wektor obiekt�w klasy CCondElem --- czyli operator�w i warto�ci. 
			Wszystkie obiekty w postaci wska�nik�w --- zarz�dza nimi. 
			Tablic� positions ustawia na pust� --- wszystkie pozycje w zdaniu zdefiniowane s� przez obiekty CCondElem.
			*/
			CSimpleCondition(std::vector<CCondElem*>& cndElems);
			/**
			\todo Obiekt zarz�dza obiektem buforu, tzn. niszczy valBufTab. 
			*/
			virtual ~CSimpleCondition(void);

		protected:
			/**
			cndElements --- wektor element�w na kt�rych dzia�a dany warunek (na razie b�d� tylko dwa, 
			ale w przysz�o�ci moga te� by� operatory 3-arg.).
			*/
			std::vector<CCondElem*>& cndElems;
		};
	}
}
#endif //LIBJOSKIPI_SIMPLEOPERATOR_H

