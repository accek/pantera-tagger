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
			/** Zbi�r metod do testowania guessera. Pozwala na podanie wi�kszo�ci parametr�w z wiersza polece�.
			Funkcjonalno��: uczenie guessera, testy (korzystaj�c z GuesserTest.h), zautomatyzowan� seri� test�w (wymaga
			pliku zawieraj�cego �cie�ki do kolejnych zestaw�w danych ucz�cych i testowych), realizacja zapyta�
			(wypluwa tagi w odpowiedzi na has�o wprowadzone przez u�ytkownika). */
			class CORPUS_API TestingConsoleUI
			{
			public:
				/// Konstruktor (nie wykonuje nic ciekawego).
				TestingConsoleUI();

				/** G��wna metoda: pozwala na wyb�r dzia�ania przez u�ytkownika. */
				bool AskAndExecute();

				/** Uczenie guessera. Najbezpieczniej jest wykona� drzewo+obcinanie, a na ewentualny
				pruning mo�na zdecydowa� si� p�niej, przy testach. W ten spos�b nie trzeba odbudowywa�
				drzewa za ka�d� zmian� parametr�w (przy pruningu). */
				bool PrepareAndSerialize();

				/// Realizacja zapyta� u�ytkownika, podgl�d zwr�conych tag�w.
				bool QueryAndAnswer();

				/** Wykonanie test�w: dok�adno��, kompletno�� i f-measure.
				Na pytania o �cie�ki do drzewa, tagset�w i s�ownika mo�na odpowiedzie� wciskaj�c ENTER
				-- wtedy u�yte zostan� standardowe �cie�ki z klasy CConfig. */
				bool DoTests();
				
				/// Test test�w.
				bool DoQuickTest();

				/** Testy tenfold na podstawie listy serii danych. Owa lista powinna sk�ada� si� ze �cie�ek
				do kolejnych danych ucz�cych i testowych, naprzemiennie (uc1, test1, uc2, test2, ...),
				ka�da �cie�ka w nowej linijce.
				Niestety �cie�ki do listy i pliku wyj�ciowego z wynikami zaszyte s� na sztywno w kodzie. */
				bool DoTenfoldTests();

				/**
				*/
				bool DoProbTests();

				/// Do przysz�ego wykorzystania.
				void MemTest();
				void TempTest();

				/** Odczytuje ci�g nazw cz�ci mowy oddzielonych znakami pustymi
				i konwertuje do maski bitowej zgodnej z formatem klasy CTag */
				static TFlexClass ReadFlexMask(std::istream &in);
				static void FillVectorWithFlexClasses(std::istream &in, std::vector<TFlexClass> &vector);
			};
		}//namespace GuesserTest
	}
}

#endif
