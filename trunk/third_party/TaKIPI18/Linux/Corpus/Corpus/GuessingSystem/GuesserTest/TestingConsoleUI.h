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

#ifndef CORPUSLIB_TESTINGCONSOLEUI_H
#define CORPUSLIB_TESTINGCONSOLEUI_H

#include "../../../corpuslib_headers.h"

#include "../../typedefs.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace GuesserTest
		{
			/** Zbiór metod do testowania guessera. Pozwala na podanie wiêkszoœci parametrów z wiersza poleceñ.
			Funkcjonalnoœæ: uczenie guessera, testy (korzystaj¹c z GuesserTest.h), zautomatyzowan¹ seriê testów (wymaga
			pliku zawieraj¹cego œcie¿ki do kolejnych zestawów danych ucz¹cych i testowych), realizacja zapytañ
			(wypluwa tagi w odpowiedzi na has³o wprowadzone przez u¿ytkownika). */
			class CORPUS_API TestingConsoleUI
			{
			public:
				/// Konstruktor (nie wykonuje nic ciekawego).
				TestingConsoleUI();

				/** G³ówna metoda: pozwala na wybór dzia³ania przez u¿ytkownika. */
				bool AskAndExecute();

				/** Uczenie guessera. Najbezpieczniej jest wykonaæ drzewo+obcinanie, a na ewentualny
				pruning mo¿na zdecydowaæ siê póŸniej, przy testach. W ten sposób nie trzeba odbudowywaæ
				drzewa za ka¿d¹ zmian¹ parametrów (przy pruningu). */
				bool PrepareAndSerialize();

				/// Realizacja zapytañ u¿ytkownika, podgl¹d zwróconych tagów.
				bool QueryAndAnswer();

				/** Wykonanie testów: dok³adnoœæ, kompletnoœæ i f-measure.
				Na pytania o œcie¿ki do drzewa, tagsetów i s³ownika mo¿na odpowiedzieæ wciskaj¹c ENTER
				-- wtedy u¿yte zostan¹ standardowe œcie¿ki z klasy CConfig. */
				bool DoTests();
				
				/// Test testów.
				bool DoQuickTest();

				/** Testy tenfold na podstawie listy serii danych. Owa lista powinna sk³adaæ siê ze œcie¿ek
				do kolejnych danych ucz¹cych i testowych, naprzemiennie (uc1, test1, uc2, test2, ...),
				ka¿da œcie¿ka w nowej linijce.
				Niestety œcie¿ki do listy i pliku wyjœciowego z wynikami zaszyte s¹ na sztywno w kodzie. */
				bool DoTenfoldTests();

				/**
				*/
				bool DoProbTests();

				/// Do przysz³ego wykorzystania.
				void MemTest();
				void TempTest();

				/** Odczytuje ci¹g nazw czêœci mowy oddzielonych znakami pustymi
				i konwertuje do maski bitowej zgodnej z formatem klasy CTag */
				static TFlexClass ReadFlexMask(std::istream &in);
				static void FillVectorWithFlexClasses(std::istream &in, std::vector<TFlexClass> &vector);
			};
		}//namespace GuesserTest
	}
}

#endif
