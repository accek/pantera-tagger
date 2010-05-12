/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_FIXSIMPOP_H
#define LIBJOSKIPI_FIXSIMPOP_H

#include "../joskipi_headers.h"
#include "SimpleOperator.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Daje dost�p do cz�ci stringa stanowi�cego warto�ci pojedynczego atrybutu
		znacznika zlokalizowane na wzgl�dnej pozycji position. Wed�ug podanej d�ugo�ci prefiksu (warto�� ujemna) 
		lub sufiksu (warto�� dodanie) odcina odpowiedni� ilo�� znak�w odpowiednio: od pocz�tku lub od ko�ca.

		\todo po zmianie vektora w CSimpleOperator zmieniono tutaj odwolania do values
		\todo poprawiono konstruktor kopiujacy i operator przypisania (tak jak w CSimpleOperator)
		*/
		class JOSKIPI_API CFixSimpOP: public CSimpleOperator
		{
		public:
			CFixSimpOP();
			CFixSimpOP(const CFixSimpOP& op);
			CFixSimpOP(pos_ptr position, int attributeIndex, int howMany) throw (std::string);
			virtual ~CFixSimpOP(void);

			const CFixSimpOP& operator=(const CFixSimpOP& op);

			/**
			Zawraca cz�� warto�� typu string, stanowi�cej warto�� okre�lonego atrybutu z okre�lonego tokenu:<br>
			1. Wed�ug pozycji ujednoznaczniania w zdaniu i wzgl�dnej pozycji position<br>
			2. Iteruj�c powszystkich znacznikach tokenu t tworzy dla atrybutu o num. attIndex:
			(a)zbi�r warto�ci typu string, kt�re s� r�wne: <br>
			* ostatnim howMany znakom warto�ci pierwotnej atrybutu, gdy howMany > 0, <br>
			* pierwszym abs(howMany) znakom, w przeciwnym wypadku<br>
			(b) gdy liczba znak�w warto�ci pierwotnej jest zbyt ma�a, to bierze wszystkie.<br>
			3. Utworzon� warto�� wstawia do val --- jako jeden obiekt \cd{CValue}, b�d� obiekty.
			*/
			virtual bool GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence);

			virtual std::string ToString();

		private:
			/**
			Ilo�� znak�w jaka ma by� zwr�cona w GetValue: dodatnia warto�� oznacza zwracanie znak�w z pocz�tku (prefiks), 
			ujemna z ko�ca (sufiks).
			*/
			int howMany;
		};
	}

}
#endif //LIBJOSKIPI_FIXSIMPOP_H

