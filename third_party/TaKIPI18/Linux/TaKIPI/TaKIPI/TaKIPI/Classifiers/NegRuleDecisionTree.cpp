#include "../../takipi_headers.h"
#include "NegRuleDecisionTree.h"
#include "../Config.h"
#include "../IO.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		CNegRuleDecisionTree::CNegRuleDecisionTree()
		{
			root = NULL;
		}

		CNegRuleDecisionTree::CNegRuleDecisionTree(std::vector<std::string>& drzewo, Corpus::TFlexClass flexClass, 
			Corpus::DLONG gramCat)
		{
			root = new CNegRuleDecisionTreeNode(drzewo);

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
		}



		CNegRuleDecisionTree::~CNegRuleDecisionTree(void)
		{
			delete root;
		}

		// Statyczna metoda do tworzenia drzewa z pliku tekstowego generowanego przez c4.5.exe
		STATIC CNegRuleDecisionTree* CNegRuleDecisionTree::CreateTree(std::ifstream& inFile, bool simplyfied, 
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

			return new CNegRuleDecisionTree(treeStrings,flexClass,gramCat);
		}

		void CNegRuleDecisionTree::Serialize()
		{
			std::stringstream namebuff;
			namebuff << IO::GetFullPath(CConfig::commonPath, CConfig::negTreeSubDir)
				<< IO::GetSeparator() << flexClass << "_" << gramaticalCat << ".bin";
			std::ofstream file;
			file.open(namebuff.str().c_str(),std::ios::binary);
			file.write((char*)&flexClass,sizeof(Corpus::DLONG));
			file.write((char*)&gramaticalCat,sizeof(Corpus::DLONG));

			root->Serialize(file);

			file.flush();
			file.close();
		}

		CNegRuleDecisionTree* CNegRuleDecisionTree::Deserialize(std::ifstream& file)
		{
			CNegRuleDecisionTree* newTree = new CNegRuleDecisionTree();

			file.read((char*)&(newTree->flexClass),sizeof(Corpus::DLONG));
			file.read((char*)&(newTree->gramaticalCat),sizeof(Corpus::DLONG));
			newTree->root = CNegRuleDecisionTreeNode::Deserialize(file);

			return newTree;
		}

		void CNegRuleDecisionTree::UseNegRuleTree(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& sentence)
		{
			Corpus::CValue v(flexClass,gramaticalCat);
			root->UseNegRuleTree(pattern,sentence,v);
		}
	}// namespace Classifiers
}//namespace TaKIPI

