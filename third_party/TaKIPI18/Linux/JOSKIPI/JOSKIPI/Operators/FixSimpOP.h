/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		Daje dostêp do czêœci stringa stanowi¹cego wartoœci pojedynczego atrybutu
		znacznika zlokalizowane na wzglêdnej pozycji position. Wed³ug podanej d³ugoœci prefiksu (wartoœæ ujemna) 
		lub sufiksu (wartoœæ dodanie) odcina odpowiedni¹ iloœæ znaków odpowiednio: od pocz¹tku lub od koñca.

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
			Zawraca czêœæ wartoœæ typu string, stanowi¹cej wartoœæ okreœlonego atrybutu z okreœlonego tokenu:<br>
			1. Wed³ug pozycji ujednoznaczniania w zdaniu i wzglêdnej pozycji position<br>
			2. Iteruj¹c powszystkich znacznikach tokenu t tworzy dla atrybutu o num. attIndex:
			(a)zbiór wartoœci typu string, które s¹ równe: <br>
			* ostatnim howMany znakom wartoœci pierwotnej atrybutu, gdy howMany > 0, <br>
			* pierwszym abs(howMany) znakom, w przeciwnym wypadku<br>
			(b) gdy liczba znaków wartoœci pierwotnej jest zbyt ma³a, to bierze wszystkie.<br>
			3. Utworzon¹ wartoœæ wstawia do val --- jako jeden obiekt \cd{CValue}, b¹dŸ obiekty.
			*/
			virtual bool GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence);

			virtual std::string ToString();

		private:
			/**
			Iloœæ znaków jaka ma byæ zwrócona w GetValue: dodatnia wartoœæ oznacza zwracanie znaków z pocz¹tku (prefiks), 
			ujemna z koñca (sufiks).
			*/
			int howMany;
		};
	}

}
#endif //LIBJOSKIPI_FIXSIMPOP_H

