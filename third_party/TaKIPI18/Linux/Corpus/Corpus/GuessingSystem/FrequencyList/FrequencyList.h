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

#ifndef CORPUSLIB_FREQUENCYLIST_H
#define CORPUSLIB_FREQUENCYLIST_H

#include "../../../corpuslib_headers.h"
namespace Corpus
{
	namespace GuessingSystem
	{
		namespace FrequencyList
		{
			// Wczytuje list� plik�w korpusu do wektora.
			CORPUS_API void LoadCorporaFile(std::string listFileName, std::vector<std::string>& list);

			// Znajduje nast�pne wyst�pienie w pliku konstrukcji "<orth>forma</orth>",
			// zwraca true jak znajdzie forme, je�li koniec pliku false.
			CORPUS_API bool FindNextOrth(std::ifstream& xmlFile, std::string& orth, bool ignoreIgn = false);

			// Dodaje dan� form� "orth" do listy "list", wraz ze zaktualizowaniem jej cz�sto�ci wyst�pienia
			// (je�li ju� istnia�a). Ostatni parametr sygnalizuje po ilu elementach dodanych do s�ownika
			// wy�wietli� informacj na konsol� (je�li 0, informacja nie jest wy�wietlana).
			CORPUS_API void AddToList(std::string& orth, std::map<std::string,int>& list, int consoleInformFrequency = 0);

			// Zapisuje list� do pliku.
			CORPUS_API void SaveList(std::map<std::string,int>& list, std::string fileName);
			// Wczytuje list� z pliku. Zak�adamy, �e kodowanie to CP1250!
			CORPUS_API void LoadList(std::map<std::string,int>& list, std::string fileName, int consoleInformFrequency = 0);
			// Dzieli list� wyraz�w na dwie listy wzgl�dem wsp�czynnika proporcji.
			CORPUS_API void DivideList(std::string fileName, std::string newFileName1, std::string newFileName, double ratio);

			// G��wna metoda tworz�ca list� cz�sto�ci form wyraz�w.
			// Ostatni parametr sygnalizuje po ilu elementach dodanych do s�ownika
			// wy�wietli� informacj na konsol� (je�li 0, informacja nie jest wy�wietlana).
			CORPUS_API void CreateFrequencyList(std::string corporaDocumentsListFile, std::string outPutFile, int consoleInformFrequency = 0, bool ignoreIgn = false);
		}
	}
}
#endif

