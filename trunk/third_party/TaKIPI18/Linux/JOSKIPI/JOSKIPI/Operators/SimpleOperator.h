/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		Daje dostêp do wartoœci pojedynczego atrybutu znacznika zlokalizowanego na
		wzglêdnej pozycji position.

		\todo dodano do operatora kopiuj¹cego i operatora = kopiowanie wektora values
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
			Zawraca wartoœæ okreœlonego atrybutu z okreœlonego tokenu.<br>
			Powinno byæ: 
			1. Wed³ug pozycji ujednoznaczniania w zdaniu i wzglêdnej pozycji position pobiera token t.
			2.Iteruj¹c po wszystkich znacznikach tokenu t tworzy dla atrybutu o num. attIndex:<br>
			(a) sumê binarn¹ w przypadku atrybutu numerycznego,<br>
			(b) zbiór wartoœci dla atrybutu stringowego tj. base i orth.<br>
			3. Utworzon¹ wartoœæ wstawia do val — jako jedeno biekt CValue, b¹dŸ obiekty.

			*/
			virtual bool GetValue(std::vector<Corpus::CValue>& val, Corpus::CSentence& sentence);

			virtual void GetValueNames(std::stringstream& out);

			virtual std::string ToString();

		protected:

			/**
			Numer atrybutu znacznika, który ma byæ odczytywany w GetValue / index atrybutu
			*/
			int attIndex;
			// Tablica wyprowadzonych wartoœci (binarnych i stringowych).
			std::vector<Corpus::DLONG> *values;
		};
	}

}
#endif

