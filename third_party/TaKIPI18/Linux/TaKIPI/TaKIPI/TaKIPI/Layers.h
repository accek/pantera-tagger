/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_LAYERS_H
#define TAKIPI_LAYERS_H

#include "../takipi_headers.h"

namespace TaKIPI
{
	class TAKIPI_DEV_API CLayers
	{
	public:
		/// Tworzy warstwy na podstawie podanych atrybut�w i ich indeks�w.
		CLayers(std::vector<int> attributes, std::vector<int> layers);
		/// Tworzy puste warstwy, do wykorzystania wraz z jedn� z funkcji SetUp...
		CLayers();
		
		~CLayers(void);
		
		/// Kasuje warstwy.
		void Clear();
		
		/** Przygotowuje standardowe warstwy; wszystkie a� do warstwy upper.
		  * Podanie upper = 2 (warto�� domy�lna) spowoduje dodanie warstw:
		  * L0 (flex), L1 (nmb,gnd), L2(cas).
		  * Podanie upper = 1 spowoduje dodanie L0 i L1, a upper = 0 -- tylko L0.
		  */
		void SetUpDefaultLayers(int upper = 2);
		
		/** Wype�nia warstwy pojedyncz� warstw� -- s�u�y do uczenia tagera.
		  * Do tagowania oraz test�w u�ywa si� SetUpDefaultLayers, poniewa�
		  * wtedy jeste�my zainteresowani dzia�aniem wszystkich dotychczasowych
		  * warstw.
		  * which to liczba z przedzia�u 0 -- 2 (L0 -- flex, L1 -- nmb,gnd, L2 -- cas).
		  */
		void SetUpDefaultSingleLayer(int which);

		const int GetCurrentAttribute() const;

		// Przechodzi do pierwszej warstwy.
		bool GotoFirstLayer();
		// Przechodzi do nast�pnej warstwy.
		bool GotoNextLayer();

		// Przechodzi do pierwszego atrybutu w danej warstwie.
		bool GotoFirst();
		// Przechodzi do nast�pnego atrybutu w danej warstwie.
		bool GotoNext();

		// Przechodzi do pierwszego atrybutu w poprzedniej warstwie.
		bool GotoPreviousFirst();
		// Przechodzi do nast�pnego atrybutu w poprzedniej warstwie.
		bool GotoPreviousNext();

		// Zwraca mask� z�o�on� z atrybut�w danej warstwy.
		void GetLayerMask(Corpus::CValue& mask, bool previous = false);
		// Zwraca mask� z�o�on� z atrybut�w dane warstwy.
		void GetAllMasks(Corpus::CValue& mask);

	private:
		// Wektor kolejno uczonych/ujednoznacznianych atrybut�w.
		std::vector<int> attributes;
		// Wektor offset�w, m�wi�cy jak si� porusza� po wektorze attrybut�w (wektor warstw).
		std::vector<int> layers;
		// Indeks obecnej warstwy.
		int layerIndex;
		// Indeks wewn�trz obecnej warstwy.
		int layerPointer;
	};

}//namespace TaKIPI
#endif //TAKIPI_LAYERS_H

