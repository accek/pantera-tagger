/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_UDICTIONARY_H
#define TAKIPI_UDICTIONARY_H

#include "../takipi_headers.h"

namespace TaKIPI
{
	/**
	S³ownik unigramowy. Zawieaj¹cy mapê obiektów <s³owo,iloœæ_lexemów> na token, 
	w którym dla ka¿dego lexemu jest zawarta informacja statystyczna mówi¹ca ile razy dany lexem rozstrzyga³
	w zbiorze tokenów o tym samym orth'cie podawanych poleceniem Add.
	*/
	class TAKIPI_DEV_API CUDictionary
	{
	public:
		/** Tworzy pusty s³ownik unigramowy bez tokenów. */
		CUDictionary(void);

		/** Destruktor. */
		~CUDictionary(void);

		/** Zwraca token lub NULL o ile zgadza siê lexCount. */
		Corpus::CToken *GetExactToken(const std::string &word, int lexCount);
		
		/** Próbuje znaleŸæ najlepiej dopasowany token do podanego
		  * (o ile jest wiêcej kandydatów dla tej formy)
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
		
		/** Zapis form powtarzaj¹cych siê. */
		void Dump(std::ostream &file);

	private:
		
		/** Odzworowanie przechowuj¹ce rzutowanie indeksu s³owa z WordDict
		  * w parê (licznoœæ leksemów, token zawieraj¹cy leksemy z licznoœciami w polu prob).
		  * Ka¿da taka para w ramach jednego s³owa ma unikaln¹ licznoœæ.
		  * Podczas odczytu stosowana jest heurystyka znajdowania najlej pary
		  * dla danego tokenu (licznoœæ ma znaczenie podczas uczenia,
		  * by zidentyfikowaæ wszystkie warianty */
		std::map< int, std::vector< std::pair<int, Corpus::CToken *> > > *dictionary;
		
		/** Funkcja pomocnicza, przygotowuje token, którego nie ma jeszcze w s³owniku.
		  * Ustawia licznoœci (pola prob leksemów) na 1 tam, gdzie by³ disamb, a pozosta³ych
		  * na zero; poza tym zeruje wszystkie disamby. */
		static void SetLexemCount(Corpus::CToken *token);
		/** Oblicza miarê podobieñstwa (f-measure) dla danych tokenów. */
		static double CompareTokens(const Corpus::CToken *dictToken, const Corpus::CToken *sentToken);
	};
}//namespace TaKIPI
#endif // TAKIPI_UDICTIONARY_H

