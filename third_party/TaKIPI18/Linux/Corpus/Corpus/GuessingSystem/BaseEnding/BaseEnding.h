/*  Biblioteka Corpus
 *  Zestaw narzędzi do przetwarzania tekstów w języku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef CORPUSLIB_BASEENDING_H
#define CORPUSLIB_BASEENDING_H

#include "../../../corpuslib_headers.h"

#include "../../Tag.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace BaseEnding
		{
			/** Wst�pnie przetwarza klas� fleksyjn� i kategorie gramatyczne przed dodaniem
			  * do drzewa podczas UCZENIA.
			  * Jest to realizacja heurystyk dotycz�cych zanegowanych imies�ow�w oraz
			  * przedrostka naj i by� mo�e kilku innych.
			  * Klasa fleksyjna oraz kategoria gram. przekazana przez referencj� mo�e zosta�
			  * zmieniona (doprowadzona do "postaci standardowej") przed umieszczeniem
			  * w drzewie. Funkcja zwraca liczb� znak�w do pomini�cia (przedrostek).
			  * Przy zgadywaniu nale�y wyj�te leksemy przepu�ci� przez PostProcess.
			  */
			CORPUS_API int PreProcess(TFlexClass &flexClass, TCategories &gramCat,
					const char *base, const char *orth);
			
			CORPUS_API int PostProcess(TFlexClass &flexClass, TCategories &gramCat,
					const char *orth);
			
			/** Sprawdza atrybuty i przedrostki orth/base i, o ile s� zgodne z podanym wzorcem,
			  * pr�buje doprowadzi� atrybuty do "postaci standardowej".
			  * Np. dla negacji przedrostek to "nie", gc_marked=gc_neg, gc_unmarked=gc_aff. */
			CORPUS_API int PreFixMarkedness(TCategories &gramCat, TCategories gc_marked, TCategories gc_unmarked,
					const char *prefix, int prefixlen, const char *base, const char *orth);
			
			/** Do u�ycia podczas odgadywania. Pr�buje odtworzy� posta� rzeczysist� na podstawie
			  * "postaci standardowej". */
			CORPUS_API int PostFixMarkedness(TCategories &gramCat, TCategories gc_marked, TCategories gc_unmarked,
					const char *prefix, int prefixlen, const char *orth);
			
			/** Znajduje ko�c�wk� formy bazowej, dodaje j� do s�ownika i zwraca (przez ref.)
			d�ugo�� ko�c�wki oraz adres w s�owniku. Parametr neg oznacza warto�� negacji leksemu.
			Je�li leksem mia� warto�� negacji "neg", by� mo�e trzeba b�dzie obci�� przedrostek "nie",
			by prawid�owo odnale�� ko�c�wk� (zgodnie z przyj�tymi regu�ami, zanegowane imies�owy
			sprowadzane s� do formy bazowej bez przedrostka "nie"). Je�li ko�c�wka zostanie uci�ta,
			zwr�cona zostanie warto�� true -- oznacza to, �e w leksemie nale�y wyzerowa� negacj�,
			tj. ustawi� jej warto�� na "aff". Dzi�ki temu nie tworzymy dw�ch r�nych ko�c�wek
			dla s��w z negacj� i bez niej. Ma to sens, poniewa� przy zgadywaniu i tak nie mamy
			wiedzy czy s�owo jest zanegowane, czy nie; a skoro to zgadujemy sami, to i tak musimy
			r�cznie ustawi� ewentualn� negacj�. TODO opis*/
			CORPUS_API void FindBaseEnding(const char *base, const char *orth,
				unsigned char &cutOffset, unsigned int &baseEndingId, int prefixToSkip);

			/** Odtwarza form� bazow� na podstawie d�ugo�ci ko�c�wki do obci�cia i adresu nowej ko�c�wki w s�owniku.
			Parametr needsNeg ma dwie funkcje. Jako parametr wej�ciowy okre�la, czy istnieje podejrzenie, �e forma
			powinna zosta� zanegowana (warto ustawi� na true, gdy leksem ma w og�le atrybut Negacja). Je�li
			wbudowana heurystyka oceni, �e jest to forma zanegowana, parametr ten zwr�ci warto�� true
			(w ka�dym innym wypadku jest ustawiany na false). TODO opis */
			CORPUS_API const std::string &GenerateBase(const char *orth, unsigned char cutOffset, unsigned int baseEndingId, int prefixToSkip);

			CORPUS_API void UpdateHistogram(int cutOffset);
			CORPUS_API void ClearStats();
			CORPUS_API void DumpStats();
		}
	}
}

#endif
