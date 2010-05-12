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

// UNIX

#ifndef _INCLUDED_GUESSERTEST_H_
#define _INCLUDED_GUESSERTEST_H_

#include "../../../corpuslib_headers.h"
#include "../Guesser.h"
#include "../../Readers/CorporaReader.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace GuesserTest
		{
			// Testy precyzji i dok�adno�ci.
			CORPUS_API void PrecisionAndRecallTests(GuessingSystem::Guesser* guesser,
				std::map<std::string,int> &testList, bool takeNodeBefIgn = true, std::ostream *outfile = NULL,
				bool countTags = true, bool countBases = false);
			CORPUS_API bool CompareLexems(CLexem *guesLex, CLexem *testLex, bool countTags, bool countBases);
			CORPUS_API void TagProbabilityTest(GuessingSystem::Guesser* guesser, std::map<std::string,int> &testList,
				Readers::CCorporaReader &corpus, std::ostream *outfile = NULL);
			CORPUS_API bool CompareLexPairs(const std::pair<CLexem *, double> &p1, const std::pair<CLexem *, double> &p2);
		}
	}
}
#endif
