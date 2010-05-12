#include "../../takipi_headers.h"
#include "DecisionTree.h"
#include "../Config.h"
#include "../IO.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		CDecisionTree::CDecisionTree(std::vector<std::string>& drzewo, Corpus::TFlexClass flexClass, 
			Corpus::DLONG gramCat)
		{
			root = new CDecisionTreeNode(drzewo);

			if(root == 0)
				throw Corpus::tagexception("Nie mo¿na by³o utworzyæ drzewa.");

			Corpus::CValue mask;
			int k = 1;
			for(int i = 2; i < (int)Corpus::CToken::END; i++)
			{
				Corpus::CTag::GetAttribMask(i,mask);
				if((mask.first & flexClass)!= 0)
					k *= Corpus::Parser::BitCount(mask.first & flexClass);
				if((mask.second & gramCat)!= 0)
					k *= Corpus::Parser::BitCount(mask.second & gramCat);
			}

			if(k == 1)
				k++;

			double lambda = ((double)(k - 1))/k;

			if(lambda == 0)
				lambda = 1;

			root->UpdateProbabilities(root->GetCount(), lambda, k);

			this->flexClass = flexClass;
			this->gramaticalCat = gramCat;

#ifdef OCR

			double sum = 0;
			int count = 0;
			root->Count(sum,count);
			this->meanProb = (double)sum/(double)count;
#endif
		}


		CDecisionTree::~CDecisionTree(void)
		{
			delete root;
		}

		/** Statyczna metoda do tworzenia drzewa z pliku tekstowego generowanego przez c4.5.exe
		  * TODO: przerobic te brzydkie zabiegi z buforem na std::getline
		  * i naprawic zla kontrola pustych linii
		  * (jesli drzewo ma pusta tresc, powstaje treeStrings == [""]). */
		STATIC CDecisionTree* CDecisionTree::CreateTree(std::ifstream& inFile, bool simplyfied, 
			Corpus::TFlexClass flexClass, Corpus::DLONG gramCat)
		{
			char* linia = new char[255];
			std::vector<std::string> treeStrings;
			std::string s;

			do
			{
				inFile.getline(linia,255);
				s = linia;
			}
			while(!inFile.eof() && s.find("Decision Tree:") == s.npos);

			inFile.getline(linia,255);
			s = linia;
			if ((int)s.length() == 0)
			{
				inFile.getline(linia,255);
				s = linia;
			}

			do
			{
				treeStrings.push_back(s);
				inFile.getline(linia,255);
				s = linia;
			}
			while(!inFile.eof() && (int)s.length() != 0);

			if(simplyfied)
			{
				do
				{
					inFile.getline(linia,255);
					s = linia;
				}
				while(!inFile.eof() && s.find("Simplified Decision Tree:") == s.npos);

				if(!inFile.eof())
				{
					treeStrings.clear();

					inFile.getline(linia,255);
					s = linia;
					if ((int)s.length() == 0)
					{
						inFile.getline(linia,255);
						s = linia;
					}

					do
					{
						treeStrings.push_back(s);
						inFile.getline(linia,255);
						s = linia;
					}
					while(!inFile.eof() && (int)s.length() != 0);
				}
			}

			inFile.close();
			delete [] linia;

			// obejscie -- gdy drzewa nie ma
			if(treeStrings.size() == 0
				|| ((treeStrings.size() == 1) && treeStrings[0].size() == 0))
				return NULL;

			return new CDecisionTree(treeStrings,flexClass,gramCat);
		}

		Corpus::CToken* CDecisionTree::Unify(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& srcSentence, 
			TaKIPI::CLayers& layers)
		{
			return root->Unify(pattern,srcSentence,layers);
		}

		void CDecisionTree::Serialize()
		{
			
			std::stringstream namebuff;
			namebuff << IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir)
				<< IO::GetSeparator() << flexClass << "_" << gramaticalCat << ".bin";
			std::ofstream file;
			file.open(namebuff.str().c_str(),std::ios::binary);

			file.write((char*)&flexClass,sizeof(Corpus::TFlexClass));
			file.write((char*)&gramaticalCat,sizeof(Corpus::DLONG));

			root->Serialize(file);

			file.flush();
			file.close();
		}

		CDecisionTree* CDecisionTree::Deserialize(std::ifstream& file)
		{
			CDecisionTree* newTree = new CDecisionTree();

			file.read((char*)&(newTree->flexClass),sizeof(Corpus::TFlexClass));
			file.read((char*)&(newTree->gramaticalCat),sizeof(Corpus::DLONG));
			newTree->root = CDecisionTreeNode::Deserialize(file);

#ifdef OCR
			double sum = 100;
			int count = 0;
			newTree->root->Count(sum,count);
			newTree->meanProb = (double)sum/(double)count;
#endif

			return newTree;
		}

	}// namespace Classifiers
}//namespace TaKIPI







