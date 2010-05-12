/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_UDICTIONARY_H
#define TAKIPI_UDICTIONARY_H

#include "../takipi_headers.h"

namespace TaKIPI
{
	/**
	S�ownik unigramowy. Zawieaj�cy map� obiekt�w <s�owo,ilo��_lexem�w> na token, 
	w kt�rym dla ka�dego lexemu jest zawarta informacja statystyczna m�wi�ca ile razy dany lexem rozstrzyga�
	w zbiorze token�w o tym samym orth'cie podawanych poleceniem Add.
	*/
	class TAKIPI_DEV_API CUDictionary
	{
	public:
		/** Tworzy pusty s�ownik unigramowy bez token�w. */
		CUDictionary(void);

		/** Destruktor. */
		~CUDictionary(void);

		/** Zwraca token lub NULL o ile zgadza si� lexCount. */
		Corpus::CToken *GetExactToken(const std::string &word, int lexCount);
		
		/** Pr�buje znale�� najlepiej dopasowany token do podanego
		  * (o ile jest wi�cej kandydat�w dla tej formy)
		  * i zwraca token lub NULL (gdy nie ma nic dla tej formy). */
		Corpus::CToken *GetToken(const Corpus::CToken *refToken);

		/** Serializacja. */
		void Serialize(std::ofstream& file);
		
		/** Zapis do pliku tekstowego. */
		void SerializeAsText(std::ostream &file);

		/** Deserializacja. */
		static CUDictionary *Deserialize(std::ifstream& file);
		
		/** Deserializacja z pliku tekstowego. */
		static CUDictionary *DeserializeFromText(std::istream &file);

		/** Dodanie formy do statystyk. */
		bool Add(Corpus::CToken* token);
		
		/** Zapis form powtarzaj�cych si�. */
		void Dump(std::ostream &file);

	private:
		
		/** Odzworowanie przechowuj�ce rzutowanie indeksu s�owa z WordDict
		  * w par� (liczno�� leksem�w, token zawieraj�cy leksemy z liczno�ciami w polu prob).
		  * Ka�da taka para w ramach jednego s�owa ma unikaln� liczno��.
		  * Podczas odczytu stosowana jest heurystyka znajdowania najlej pary
		  * dla danego tokenu (liczno�� ma znaczenie podczas uczenia,
		  * by zidentyfikowa� wszystkie warianty */
		std::map< int, std::vector< std::pair<int, Corpus::CToken *> > > *dictionary;
		
		/** Funkcja pomocnicza, przygotowuje token, kt�rego nie ma jeszcze w s�owniku.
		  * Ustawia liczno�ci (pola prob leksem�w) na 1 tam, gdzie by� disamb, a pozosta�ych
		  * na zero; poza tym zeruje wszystkie disamby. */
		static void SetLexemCount(Corpus::CToken *token);
		/** Oblicza miar� podobie�stwa (f-measure) dla danych token�w. */
		static double CompareTokens(const Corpus::CToken *dictToken, const Corpus::CToken *sentToken);
	};
}//namespace TaKIPI
#endif // TAKIPI_UDICTIONARY_H

