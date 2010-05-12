/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_CONDITION_H
#define LIBJOSKIPI_CONDITION_H

#include "../joskipi_headers.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Abstrakcyjny korze� hierarchii definiuj�cej obiekty implementuj�ce warunki na�o�one na posta� zdania.
		*/
		class JOSKIPI_API CCondition
		{
		public:
			CCondition(std::vector<pos_ptr>* positions);
			virtual ~CCondition(void);

			/**
			Abstrakcyjna, wirtualna metoda (do przes�aniania), dla podanego zdania zwraca warto�� 
			logiczn� b�d�c� wynikiem zastosowania testu logicznego.
			*/
			virtual bool Evaluate(Corpus::CSentence& sentence) = 0;

			virtual std::string ToString() = 0;

		protected:
			/**
			Tablica (zawsze b�dzie wiadomo ile ich jest) wska�nik�w do obiekt�w klasy CPosition wskazuj�cych 
			istotne po�o�enia wzgl�dne w zdaniu, do kt�rego stosowany jest warunek; obiekty te przez referencj� 
			mog� �przenosi� warto�ci pozycji pomi�dzy obiektami klasy pochodnej od CCondition; <br>
			tablica ta dla wielu operator�w b�dzie pusta; <br>
			Uwaga: obiekt�w tych nie wolno tu usuwa� � poniewa� mog� by� wsp�dzielone s� po utworzeniu 
			rejestrowane w COperMemMangager (dost�p przez metod� COperFramework ) i to on je b�dzie usuwa� 
			po zako�czeniu dzia�ania ca�ego obiektu

			\todo jest wektor, spr�bowa� przerobi� na tablic� (wydajno��);
			*/
			std::vector<pos_ptr>* posVec;
		};
	}
}

#endif //LIBJOSKIPI_CONDITION_H

