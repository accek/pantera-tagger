#include "../../../corpuslib_headers.h"

#include "TreeHistogram.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace GuesserTest
		{
			void TreeHistogram::UpdateRec(EndingTree::EndingTreeNode *node, UINTEGER currentDepth)
			{
				if(node->count_ != 0) // jeœli wêze³ ma przypisane tagsety
				{
					FlexLen blueprint; // wzorzec do wyszukiwania w zbiorze histodata
					EndingTagSet::EndingTagSet *tagSet = (*tagSets)[node->count_ - 1];
					UINTEGER sum = 0;
					for(UINTEGER i = 0; i < tagSet->count_; i++)
					{
						EndingTagSet::TagCount *tagCount = tagSet->tagsCounts_[i];
						sum += tagCount->count;
						// przygotowujemy wzorzec
						blueprint.length = currentDepth;
						blueprint.flexClass = tagCount->flexClass;
						// szukamy go
						std::map<FlexLen, UINTEGER, FlexLenComparator>::iterator found =
							histodata->find(blueprint);
						if(found == histodata->end()) // nie ma, tworzymy nowy
						{
							histodata->insert(std::pair<FlexLen, UINTEGER>(blueprint, tagCount->count));
						}
						else // klucz istnieje, zwiêkszamy licznoœæ
						{
							found->second += tagCount->count;
						}
					}

					totalHits += sum;
				}

				// jeœli s¹ dzieci, zag³êbiamy siê rekurencyjnie
				for(UBYTE i = 0; i < node->childsCount_; i++)
				{
					UpdateRec(node->childsNodes_[i], currentDepth + 1);
				}
			}

			void TreeHistogram::Dump(std::ostream &out)
			{
				out << "Wszystkich tagow: " << totalHits << std::endl;

				for(std::map<FlexLen, UINTEGER, FlexLenComparator>::iterator iter = 
					histodata->begin(); iter != histodata->end(); ++iter)
				{
					iter->first.Dump(out);
					out << "\t" << iter->second << std::endl;
				}
			}

			void TreeHistogram::GetAllRec(std::ostream &out, EndingTree::EndingTreeNode *node, std::string soFar)
			{
				// jeœli s¹ dzieci, zag³êbiamy siê rekurencyjnie
				for(UBYTE i = 0; i < node->childsCount_; i++)
				{
					GetAllRec(out, node->childsNodes_[i], soFar + node->childMapChars_[i]);
				}
				
				if(node->childsCount_ == 0)
				{
					out << soFar << "\n";
				}
			}
		} // namespace GuesserTest
	}
}
