/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_SIMPLEOPER_H
#define LIBJOSKIPI_SIMPLEOPER_H

#include "../joskipi_headers.h"
#include "Operator.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Daje dost�p do warto�ci pojedynczego atrybutu znacznika zlokalizowanego na
		wzgl�dnej pozycji position.

		\todo dodano do operatora kopiuj�cego i operatora = kopiowanie wektora values
		\todo zmieniono values ze statycznego wektora, na wektor dynamiczny
		*/
		class JOSKIPI_API CSimpleOperator :	public COperator
		{
		public:
			CSimpleOperator();
			CSimpleOperator(const CSimpleOperator& op);
			CSimpleOperator(pos_ptr position, int attributeIndex) throw(std::string);
			virtual ~CSimpleOperator(void);

			const CSimpleOperator& operator=(const CSimpleOperator& op);

			/**
			Zawraca warto�� okre�lonego atrybutu z okre�lonego tokenu.<br>
			Powinno by�: 
			1. Wed�ug pozycji ujednoznaczniania w zdaniu i wzgl�dnej pozycji position pobiera token t.
			2.Iteruj�c po wszystkich znacznikach tokenu t tworzy dla atrybutu o num. attIndex:<br>
			(a) sum� binarn� w przypadku atrybutu numerycznego,<br>
			(b) zbi�r warto�ci dla atrybutu stringowego tj. base i orth.<br>
			3. Utworzon� warto�� wstawia do val � jako jedeno biekt CValue, b�d� obiekty.

			*/
			virtual bool GetValue(std::vector<Corpus::CValue>& val, Corpus::CSentence& sentence);

			virtual void GetValueNames(std::stringstream& out);

			virtual std::string ToString();

		protected:

			/**
			Numer atrybutu znacznika, kt�ry ma by� odczytywany w GetValue / index atrybutu
			*/
			int attIndex;
			// Tablica wyprowadzonych warto�ci (binarnych i stringowych).
			std::vector<Corpus::DLONG> *values;
		};
	}

}
#endif

