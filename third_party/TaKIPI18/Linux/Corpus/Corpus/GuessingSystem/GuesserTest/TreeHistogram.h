/*  Biblioteka Corpus
 *  Zestaw narzƒôdzi do przetwarzania tekst√≥w w jƒôzyku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc≈Çawskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardy≈Ñski
 *
 *  Kod na licencji GPL.
 */

#ifndef CORPUSLIB_TREEHISTOGRAM_H
#define CORPUSLIB_TREEHISTOGRAM_H

#include "../../../corpuslib_headers.h"
#include "../../typedefs.h"
#include "../../Tag.h"
#include "../EndingTree/EndingTreeNode.h"
#include "../EndingTagSet/EndingTagSet.h"


#define TREE_HISTOGRAM_SIZE (18)

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace GuesserTest
		{
			struct CORPUS_API FlexLen
			{
				TFlexClass flexClass;
				USHORT length;

				void Dump(std::ostream &out) const
				{
					out << CTag::GetFlexClassString(flexClass)
						<< "\t" << length;
				}
			};

			struct CORPUS_API FlexLenComparator
			{
				bool operator()(const FlexLen &f1, const FlexLen &f2) const
				{
					return f1.flexClass < f2.flexClass
					|| (f1.flexClass == f2.flexClass && f1.length < f2.length);
				}
			};

			// Histogram d≥ugoúci koÒcÛwek wg tagsetÛw
			class CORPUS_API TreeHistogram
			{
			public:
				TreeHistogram(
					EndingTree::EndingTreeNode* treeToExamine,
					std::vector<GuessingSystem::EndingTagSet::EndingTagSet*> *tagSetVector):
					tree(treeToExamine), tagSets(tagSetVector), totalHits(0)
				{
					histodata = new std::map<FlexLen, UINTEGER, FlexLenComparator>();
				}

				~TreeHistogram()
				{
					delete histodata;
				}

				void Update()
				{
					UpdateRec(tree, 0);
				}

				void Clear()
				{
					totalHits = 0;
					histodata->clear();
				}

				void Dump(std::ostream &out);
				void GetAll(std::ostream &out)
				{
					std::string empty;
					GetAllRec(out, tree, empty);
				}
				

			private:
				EndingTree::EndingTreeNode *tree;
				std::vector<GuessingSystem::EndingTagSet::EndingTagSet*> *tagSets;
				UINTEGER totalHits;
				//std::set<FlexCount, FlexCountComparator> histodata;
				std::map<FlexLen, UINTEGER, FlexLenComparator> *histodata;

				void UpdateRec(EndingTree::EndingTreeNode *node, UINTEGER currentDepth);
				void GetAllRec(std::ostream &out, EndingTree::EndingTreeNode *node, std::string soFar);
			};
		}
	}
}

#endif
