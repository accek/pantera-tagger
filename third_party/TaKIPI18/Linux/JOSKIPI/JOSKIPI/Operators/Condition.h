/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		Abstrakcyjny korzeñ hierarchii definiuj¹cej obiekty implementuj¹ce warunki na³o¿one na postaæ zdania.
		*/
		class JOSKIPI_API CCondition
		{
		public:
			CCondition(std::vector<pos_ptr>* positions);
			virtual ~CCondition(void);

			/**
			Abstrakcyjna, wirtualna metoda (do przes³aniania), dla podanego zdania zwraca wartoœæ 
			logiczn¹ bêd¹c¹ wynikiem zastosowania testu logicznego.
			*/
			virtual bool Evaluate(Corpus::CSentence& sentence) = 0;

			virtual std::string ToString() = 0;

		protected:
			/**
			Tablica (zawsze bêdzie wiadomo ile ich jest) wskaŸników do obiektów klasy CPosition wskazuj¹cych 
			istotne po³o¿enia wzglêdne w zdaniu, do którego stosowany jest warunek; obiekty te przez referencjê 
			mog¹ ‘przenosiæ’ wartoœci pozycji pomiêdzy obiektami klasy pochodnej od CCondition; <br>
			tablica ta dla wielu operatorów bêdzie pusta; <br>
			Uwaga: obiektów tych nie wolno tu usuwaæ — poniewa¿ mog¹ byæ wspó³dzielone s¹ po utworzeniu 
			rejestrowane w COperMemMangager (dostêp przez metodê COperFramework ) i to on je bêdzie usuwa³ 
			po zakoñczeniu dzia³ania ca³ego obiektu

			\todo jest wektor, spróbowaæ przerobiæ na tablicê (wydajnoœæ);
			*/
			std::vector<pos_ptr>* posVec;
		};
	}
}

#endif //LIBJOSKIPI_CONDITION_H

