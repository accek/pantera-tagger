/*  Biblioteka Corpus
 *  Zestaw narzÄ™dzi do przetwarzania tekstÃ³w w jÄ™zyku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
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
			/** Wstêpnie przetwarza klasê fleksyjn¹ i kategorie gramatyczne przed dodaniem
			  * do drzewa podczas UCZENIA.
			  * Jest to realizacja heurystyk dotycz¹cych zanegowanych imies³owów oraz
			  * przedrostka naj i byæ mo¿e kilku innych.
			  * Klasa fleksyjna oraz kategoria gram. przekazana przez referencjê mo¿e zostaæ
			  * zmieniona (doprowadzona do "postaci standardowej") przed umieszczeniem
			  * w drzewie. Funkcja zwraca liczbê znaków do pominiêcia (przedrostek).
			  * Przy zgadywaniu nale¿y wyjête leksemy przepuœciæ przez PostProcess.
			  */
			CORPUS_API int PreProcess(TFlexClass &flexClass, TCategories &gramCat,
					const char *base, const char *orth);
			
			CORPUS_API int PostProcess(TFlexClass &flexClass, TCategories &gramCat,
					const char *orth);
			
			/** Sprawdza atrybuty i przedrostki orth/base i, o ile s¹ zgodne z podanym wzorcem,
			  * próbuje doprowadziæ atrybuty do "postaci standardowej".
			  * Np. dla negacji przedrostek to "nie", gc_marked=gc_neg, gc_unmarked=gc_aff. */
			CORPUS_API int PreFixMarkedness(TCategories &gramCat, TCategories gc_marked, TCategories gc_unmarked,
					const char *prefix, int prefixlen, const char *base, const char *orth);
			
			/** Do u¿ycia podczas odgadywania. Próbuje odtworzyæ postaæ rzeczysist¹ na podstawie
			  * "postaci standardowej". */
			CORPUS_API int PostFixMarkedness(TCategories &gramCat, TCategories gc_marked, TCategories gc_unmarked,
					const char *prefix, int prefixlen, const char *orth);
			
			/** Znajduje koñcówkê formy bazowej, dodaje j¹ do s³ownika i zwraca (przez ref.)
			d³ugoœæ koñcówki oraz adres w s³owniku. Parametr neg oznacza wartoœæ negacji leksemu.
			Jeœli leksem mia³ wartoœæ negacji "neg", byæ mo¿e trzeba bêdzie obci¹æ przedrostek "nie",
			by prawid³owo odnaleŸæ koñcówkê (zgodnie z przyjêtymi regu³ami, zanegowane imies³owy
			sprowadzane s¹ do formy bazowej bez przedrostka "nie"). Jeœli koñcówka zostanie uciêta,
			zwrócona zostanie wartoœæ true -- oznacza to, ¿e w leksemie nale¿y wyzerowaæ negacjê,
			tj. ustawiæ jej wartoœæ na "aff". Dziêki temu nie tworzymy dwóch ró¿nych koñcówek
			dla s³ów z negacj¹ i bez niej. Ma to sens, poniewa¿ przy zgadywaniu i tak nie mamy
			wiedzy czy s³owo jest zanegowane, czy nie; a skoro to zgadujemy sami, to i tak musimy
			rêcznie ustawiæ ewentualn¹ negacjê. TODO opis*/
			CORPUS_API void FindBaseEnding(const char *base, const char *orth,
				unsigned char &cutOffset, unsigned int &baseEndingId, int prefixToSkip);

			/** Odtwarza formê bazow¹ na podstawie d³ugoœci koñcówki do obciêcia i adresu nowej koñcówki w s³owniku.
			Parametr needsNeg ma dwie funkcje. Jako parametr wejœciowy okreœla, czy istnieje podejrzenie, ¿e forma
			powinna zostaæ zanegowana (warto ustawiæ na true, gdy leksem ma w ogóle atrybut Negacja). Jeœli
			wbudowana heurystyka oceni, ¿e jest to forma zanegowana, parametr ten zwróci wartoœæ true
			(w ka¿dym innym wypadku jest ustawiany na false). TODO opis */
			CORPUS_API const std::string &GenerateBase(const char *orth, unsigned char cutOffset, unsigned int baseEndingId, int prefixToSkip);

			CORPUS_API void UpdateHistogram(int cutOffset);
			CORPUS_API void ClearStats();
			CORPUS_API void DumpStats();
		}
	}
}

#endif
