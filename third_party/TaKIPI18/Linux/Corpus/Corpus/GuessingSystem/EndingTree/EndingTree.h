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

#ifndef CORPUSLIB_ENDINGTREE_H
#define CORPUSLIB_ENDINGTREE_H

#include "../../../corpuslib_headers.h"
#include "../../typedefs.h"
#include "EndingTreeNode.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace EndingTree
		{
			// Tworzy drzewo końcówek z listy frekwencyjnej.
			CORPUS_API EndingTreeNode* CreateEndingTree(STRING2INT_MAP &list, int consoleInformFrequency = 0);
		}
	}
}
#endif
