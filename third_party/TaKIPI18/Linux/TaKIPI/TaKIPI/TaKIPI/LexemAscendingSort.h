/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_LEXEMASCENDINGSORT_H
#define TAKIPI_LEXEMASCENDINGSORT_H

#include "../takipi_headers.h"
#include "Layers.h"

namespace TaKIPI
{
	/*
	Klasa realizuj�ca por�wnywanie dw�ch lexem�w za pomoc�, u�ywana w algorytmie sortowania STL.
	Sortowanie nastepuje po warstwach przekazywanych w konstruktorze.
	*/
	class TAKIPI_DEV_API CLexemAscendingSort
	{
	public:
		/*
		Konstruktor tworz�cy por�wnywacz, do kt�rego przekazywany jest obiekt reprezentuj�cy warstwy
		zgodnie z kt�rymi b�dziemy sortowa�.
		*/
		CLexemAscendingSort(TaKIPI::CLayers& layers);
		~CLexemAscendingSort(void);

		/*
		Sortowanie lexem�w.	Je�li w obiekcie layers podanym w kontruktorze nie zdefiniowano warstw, wyrzuca wyj�tek.
		W przeciwnym wypadku por�wnuje warto�ci poszczeg�lnych atrybut�w w warstwach i wzgl�dem tego sortuje.
		*/
		bool operator()(Corpus::CLexem* l1, Corpus::CLexem* l2);

		// Informacje o warstwie w jakiej si� znajdujemy.
		TaKIPI::CLayers& layers;
	};
}//namespace TaKIPI

#endif //TAKIPI_LEXEMASCENDINGSORT_H

