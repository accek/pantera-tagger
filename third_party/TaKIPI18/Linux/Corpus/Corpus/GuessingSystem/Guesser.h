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

#ifndef CORPUSLIB_GUESSER_H_
#define CORPUSLIB_GUESSER_H_

#include "../../corpuslib_headers.h"

#include "../Token.h"

#include "EndingTree/EndingTree.h"
#include "EndingTree/EndingTreeNode.h"
#include "EndingTagSet/EndingTagSet.h"


namespace Corpus
{
	namespace GuessingSystem
	{
		class CORPUS_API Guesser
		{
		public:
			Guesser(const char* formsListFile);
			Guesser(const char* serializedEndingTreeFile, const char* serializedEndingTagSetVectorFile);
			~Guesser();

			// Uog�lnianie drzewa (przycinanie).
			void Pruning(double weightedSpecificityRatio);
			void Pruning2(double weightedSpecificityRatio);
			// Przeprowadza analiz� morfologiczn� formy.
			CToken* Guess(const char* form, bool takeNodeBeforeIgn = true);
			// Serializacja drzewa (plik drzewa i plik listy)
			void Serialize(const char* serializedEndingTreeFile, const char* serializedEndingTagSetVectorFile);

			void AttachLeafToIgn();

			void DumpTreeStats(std::ostream &out);
			void DumpTreePaths(std::ostream &out);

		private:
			// Drzewo identyfikacji ko�c�wek
			GuessingSystem::EndingTree::EndingTreeNode* tree_;
			// Lista tagset�w dla ko�cowek.
#ifndef LINUX
	template class CORPUS_API std::allocator<GuessingSystem::EndingTagSet::EndingTagSet*>;
	template class CORPUS_API std::vector<GuessingSystem::EndingTagSet::EndingTagSet*, std::allocator<GuessingSystem::EndingTagSet::EndingTagSet*> >;
#endif
			std::vector<GuessingSystem::EndingTagSet::EndingTagSet*> outList_;
		}; // class Guesser
	} // namespace GuessingSystem
} // namespace Corpus
#endif
