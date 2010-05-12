/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
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
	Klasa realizuj¹ca porównywanie dwóch lexemów za pomoc¹, u¿ywana w algorytmie sortowania STL.
	Sortowanie nastepuje po warstwach przekazywanych w konstruktorze.
	*/
	class TAKIPI_DEV_API CLexemAscendingSort
	{
	public:
		/*
		Konstruktor tworz¹cy porównywacz, do którego przekazywany jest obiekt reprezentuj¹cy warstwy
		zgodnie z którymi bêdziemy sortowaæ.
		*/
		CLexemAscendingSort(TaKIPI::CLayers& layers);
		~CLexemAscendingSort(void);

		/*
		Sortowanie lexemów.	Jeœli w obiekcie layers podanym w kontruktorze nie zdefiniowano warstw, wyrzuca wyj¹tek.
		W przeciwnym wypadku porównuje wartoœci poszczególnych atrybutów w warstwach i wzglêdem tego sortuje.
		*/
		bool operator()(Corpus::CLexem* l1, Corpus::CLexem* l2);

		// Informacje o warstwie w jakiej siê znajdujemy.
		TaKIPI::CLayers& layers;
	};
}//namespace TaKIPI

#endif //TAKIPI_LEXEMASCENDINGSORT_H

