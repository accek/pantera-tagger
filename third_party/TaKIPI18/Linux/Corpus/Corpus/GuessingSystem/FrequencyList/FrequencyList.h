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

#ifndef CORPUSLIB_FREQUENCYLIST_H
#define CORPUSLIB_FREQUENCYLIST_H

#include "../../../corpuslib_headers.h"
namespace Corpus
{
	namespace GuessingSystem
	{
		namespace FrequencyList
		{
			// Wczytuje listê plików korpusu do wektora.
			CORPUS_API void LoadCorporaFile(std::string listFileName, std::vector<std::string>& list);

			// Znajduje nastêpne wyst¹pienie w pliku konstrukcji "<orth>forma</orth>",
			// zwraca true jak znajdzie forme, jeœli koniec pliku false.
			CORPUS_API bool FindNextOrth(std::ifstream& xmlFile, std::string& orth, bool ignoreIgn = false);

			// Dodaje dan¹ formê "orth" do listy "list", wraz ze zaktualizowaniem jej czêstoœci wyst¹pienia
			// (jeœli ju¿ istnia³a). Ostatni parametr sygnalizuje po ilu elementach dodanych do s³ownika
			// wyœwietliæ informacj na konsolê (jeœli 0, informacja nie jest wyœwietlana).
			CORPUS_API void AddToList(std::string& orth, std::map<std::string,int>& list, int consoleInformFrequency = 0);

			// Zapisuje listê do pliku.
			CORPUS_API void SaveList(std::map<std::string,int>& list, std::string fileName);
			// Wczytuje listê z pliku. Zak³adamy, ¿e kodowanie to CP1250!
			CORPUS_API void LoadList(std::map<std::string,int>& list, std::string fileName, int consoleInformFrequency = 0);
			// Dzieli listê wyrazów na dwie listy wzglêdem wspó³czynnika proporcji.
			CORPUS_API void DivideList(std::string fileName, std::string newFileName1, std::string newFileName, double ratio);

			// G³ówna metoda tworz¹ca listê czêstoœci form wyrazów.
			// Ostatni parametr sygnalizuje po ilu elementach dodanych do s³ownika
			// wyœwietliæ informacj na konsolê (jeœli 0, informacja nie jest wyœwietlana).
			CORPUS_API void CreateFrequencyList(std::string corporaDocumentsListFile, std::string outPutFile, int consoleInformFrequency = 0, bool ignoreIgn = false);
		}
	}
}
#endif

