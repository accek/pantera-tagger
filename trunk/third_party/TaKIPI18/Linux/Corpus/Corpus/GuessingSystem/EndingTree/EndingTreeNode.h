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

#ifndef CORPUSLIB_ENDINGTREENODE_H
#define CORPUSLIB_ENDINGTREENODE_H

#include "../../../corpuslib_headers.h"
#include "../../typedefs.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace EndingTree
		{
			struct CORPUS_API EndingTreeNode
			{
				// Flaga wstępnie do informacji na temat kończenia się wyrazu w danym węźle.
				USHORT flag_;
				// Liczba form posiadających końcówkę reprezentowaną przez dany węzeł.
				UINTEGER count_;

				// Liczba potomków.
				UBYTE childsCount_;
				// Tablica odwzorowań.
				char* childMapChars_;
				// Podgałęzie drzewa.
				EndingTreeNode** childsNodes_;

				// Rozwija drzewo według danej formy.
				void AddForm(const char* form);
				// Dodawanie poddrzewa.
				void AddSubTree(char character);
				// Usuwanie poddrzewa.
				void CutSubTree(char character);
				// Usuwanie poddrzewa.
				void CutSubTree(INTEGER childNumber);
				// Zwraca wsaźnik na poddrzewo lub NULL gdy poddrzewo dla danej litery nie istnieje.
				EndingTreeNode* GetSubTree(char character);

				// Kasuje drzewo wraz z poddrzewami.
				void Free();
				// Sprawdza czy można obciąć tę gałąź drzewa (czy jest "sierotą").
				bool CanBeCut();
				// Wycina "sieroty".
				void CutOrphans();
				// Czyści liczności (będą potem użyte jako identyfikator)
				void ClearCount();

				// Znajduje najdłuższą pasującą końcówkę.
				std::string FindLongestEnding(const char* form);
				// Zwraca referencję do węzła o najdłuższej pasującej końcówce.
				EndingTreeNode* FindLongestEndingRef(const char* form, bool takeNodeBefIgn = false);

				// Serializacja
				void Serialize(std::ofstream& file);
			};

			// Deserializacja
			CORPUS_API EndingTreeNode* Deserialize(std::ifstream& file);
		}
	}
}
#endif

