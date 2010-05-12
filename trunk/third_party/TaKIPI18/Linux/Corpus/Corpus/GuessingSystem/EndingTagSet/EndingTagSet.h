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

#ifndef CORPUSLIB_ENDINGTAGSET_H
#define CORPUSLIB_ENDINGTAGSET_H

#include "../../../corpuslib_headers.h"

#include "../../typedefs.h"
#include "../../Token.h"
#include "../EndingTree/EndingTree.h"
#include "../BaseEnding/BaseEnding.h"

namespace Corpus
{
	class CToken;
	namespace GuessingSystem
	{
		namespace EndingTagSet
		{
			struct CORPUS_API TagCount
			{
				DLONG flexClass;
				DLONG gramCat;
				UINTEGER count;
				/// ile znakow trzeba obciac, zanim dodamy koncowke formy bazowej
				UBYTE cutOffset;
				/// identyfikator koncowki formy bazowej
				UINTEGER baseEndingId;

				/// Konstruktor dla deserializacji.
				TagCount(std::ifstream &file);

				TagCount(DLONG flex, DLONG cat, UINTEGER c, UBYTE offset, UINTEGER id):
					flexClass(flex), gramCat(cat), count(c), cutOffset(offset), baseEndingId(id)
				{
				}

				void Serialize(std::ofstream &file);
			};

			struct CORPUS_API EndingTagSet
			{
				// Identyfikator danego tagsetu
				UINTEGER offset;

				// Liczność tagów
				UINTEGER count_;
				// Lista tagów
				TagCount** tagsCounts_;

				// Konstruktor
				EndingTagSet(): tagsCounts_(NULL) {};
				EndingTagSet(std::ifstream &file);

				// Destruktor -- zwalnia tablicę tagsCounts (obiekty TagCount).
				~EndingTagSet();

				// Dodawanie tagsetu słowa.
				void AddTags(const std::string &form, int formCount);
				// Dodawanie tagu.
				void AddNewTagCount(TagCount *tagCount);
				// Dodawnaie tagu (może istnieć w wektorze, liczności są dodawane).
				void AddTag(TagCount* tag);
				// Znajdowanie tagu (-1 => tag nie istnieje)
				int FindTag(DLONG flexClass, DLONG gramCat);

				// Serializacja
				void Serialize(std::ofstream& file);
			};

			// Serializacja i Deserializacja
			CORPUS_API void Deserialize(std::ifstream& file, std::vector<EndingTagSet*>& endingVector);
			CORPUS_API void Serialize(std::ofstream& file, std::vector<EndingTagSet*>& endingVector);

			// Tworzy listę końcówek wraz z tagsetami
			CORPUS_API void CreateEndingTagSetsList(EndingTree::EndingTreeNode* tree, STRING2INT_MAP &formsList,
										std::vector<EndingTagSet*>& outList, int consoleInformFrequency = 0);

			// Przycinanie drzewa
			// - weightedSpecificityRatio - określa próg szczegółowości tagów pozwalający na przycinanie.
			CORPUS_API void Pruning(EndingTree::EndingTreeNode* tree, double weightedSpecificityRatio, std::vector<EndingTagSet*>& tagSetsList);
			CORPUS_API void Pruning2(EndingTree::EndingTreeNode* tree, double weightedSpecificityRatio, std::vector<EndingTagSet*>& tagSetsList);

			/// Metoda dla węzła nie posiadającego kokońcówki przypisuje końcówkę któregoś z jego dzieci będącego liściem.
			CORPUS_API void AttachLeafToIgn(EndingTree::EndingTreeNode* tree);

			/// Zgaduje analizę morfologiczną formy.
			CORPUS_API CToken* GuessForm(const char* form, EndingTree::EndingTreeNode* tree, std::vector<EndingTagSet*>& outList, bool takeNodeBefIgn = false);

		}
	}
}
#endif
