/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
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
		/// Tworzy warstwy na podstawie podanych atrybutów i ich indeksów.
		CLayers(std::vector<int> attributes, std::vector<int> layers);
		/// Tworzy puste warstwy, do wykorzystania wraz z jedn¹ z funkcji SetUp...
		CLayers();
		
		~CLayers(void);
		
		/// Kasuje warstwy.
		void Clear();
		
		/** Przygotowuje standardowe warstwy; wszystkie a¿ do warstwy upper.
		  * Podanie upper = 2 (wartoœæ domyœlna) spowoduje dodanie warstw:
		  * L0 (flex), L1 (nmb,gnd), L2(cas).
		  * Podanie upper = 1 spowoduje dodanie L0 i L1, a upper = 0 -- tylko L0.
		  */
		void SetUpDefaultLayers(int upper = 2);
		
		/** Wype³nia warstwy pojedyncz¹ warstw¹ -- s³u¿y do uczenia tagera.
		  * Do tagowania oraz testów u¿ywa siê SetUpDefaultLayers, poniewa¿
		  * wtedy jesteœmy zainteresowani dzia³aniem wszystkich dotychczasowych
		  * warstw.
		  * which to liczba z przedzia³u 0 -- 2 (L0 -- flex, L1 -- nmb,gnd, L2 -- cas).
		  */
		void SetUpDefaultSingleLayer(int which);

		const int GetCurrentAttribute() const;

		// Przechodzi do pierwszej warstwy.
		bool GotoFirstLayer();
		// Przechodzi do nastêpnej warstwy.
		bool GotoNextLayer();

		// Przechodzi do pierwszego atrybutu w danej warstwie.
		bool GotoFirst();
		// Przechodzi do nastêpnego atrybutu w danej warstwie.
		bool GotoNext();

		// Przechodzi do pierwszego atrybutu w poprzedniej warstwie.
		bool GotoPreviousFirst();
		// Przechodzi do nastêpnego atrybutu w poprzedniej warstwie.
		bool GotoPreviousNext();

		// Zwraca maskê z³o¿on¹ z atrybutów danej warstwy.
		void GetLayerMask(Corpus::CValue& mask, bool previous = false);
		// Zwraca maskê z³o¿on¹ z atrybutów dane warstwy.
		void GetAllMasks(Corpus::CValue& mask);

	private:
		// Wektor kolejno uczonych/ujednoznacznianych atrybutów.
		std::vector<int> attributes;
		// Wektor offsetów, mówi¹cy jak siê poruszaæ po wektorze attrybutów (wektor warstw).
		std::vector<int> layers;
		// Indeks obecnej warstwy.
		int layerIndex;
		// Indeks wewn¹trz obecnej warstwy.
		int layerPointer;
	};

}//namespace TaKIPI
#endif //TAKIPI_LAYERS_H

