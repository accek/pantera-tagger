#include "../../../corpuslib_headers.h"
#include "EndingTree.h"
#include "../../Parser/Parser.h"
#include "../../Morfeusz/morfeusz.h"
#include "../BaseEnding/Prefixes.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace EndingTree
		{

			// Tworzy drzewo koñcówek z listy frekwencyjnej.
			EndingTreeNode* CreateEndingTree(STRING2INT_MAP &list, int consoleInformFrequency)
			{
				std::cerr << "Rozpoczecie tworzenia drzewa." << std::endl;
				EndingTreeNode* root = new EndingTreeNode();

				STRING2INT_MAP::iterator formIter = list.begin();
				int j = 0;
				for(; formIter != list.end(); formIter++)
				{
					// heurystyka wy³¹czona
					//std::string postForm = BaseEnding::Prefixes::GetInstance()->ProcessPrefixes(formIter->first);
					std::string postForm = formIter->first;
					reverse(postForm.begin(),postForm.end());
					root->AddForm(postForm.c_str());
					j++;
					if(consoleInformFrequency != 0
						&& (j % consoleInformFrequency) == 0)
						std::cerr << "\r- dodano " << j << " form do drzewa";
				}

				std::cerr << "\nZakonczenie tworzenia drzewa." << std::endl;
				return root;
			}
		}
	}
}
