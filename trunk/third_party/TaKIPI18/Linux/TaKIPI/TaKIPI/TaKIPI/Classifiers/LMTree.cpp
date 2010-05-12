#include "../../takipi_headers.h"
#include "LMTree.h"
#include "../IO.h"

namespace TaKIPI
{
	namespace Classifiers
	{

		CLMTree::CLMTree(std::vector<std::string>& treedata, std::vector< std::vector<std::string> > & modelsdata)
		{
			models = new CLogisticModel[modelsdata.size()];
			for(size_t i = 0; i < modelsdata.size(); i++)
			{
				models[i].Parse(modelsdata[i]);
#ifdef _DEBUG
				std::string s = models[i].ToString();
#endif
			}
			root = new CLMTreeNode(treedata,models);

			if(root == NULL)
				throw Corpus::tagexception("Nie mo¿na by³o utworzyæ drzewa.");

		}
		CLMTree::~CLMTree(void)
		{
			delete root;
			delete models;
		}

		// Statyczna metoda do tworzenia drzewa z pliku tekstowego generowanego przez LMTLearner
		STATIC CLMTree* CLMTree::CreateTree(std::ifstream& inFile)
		{
			std::vector<std::string> treeStrings;
			std::string s;	
			do
			{
				s = IO::ReadLine(inFile);
			}
			while(!inFile.eof() && (s.find("Logistic model tree") == s.npos));

			IO::ReadLine(inFile);// "------------------"
			s = IO::ReadLine(inFile);

			while(!inFile.eof() && (s.find("Number of Leaves") == s.npos))
			{
				treeStrings.push_back(s);
				s = IO::ReadLine(inFile);
			}
			if(inFile.eof())
			{
				return NULL;
			}
			treeStrings.pop_back();//usuniecie ostatniej pustej linijki
			std::vector<std::string> tokens;
			Corpus::Parser::Tokenize(s,tokens,":");
			if(tokens.size() != 2)
				throw Corpus::tagexception("CLMTree::CreateTree, expected line \"Number of Leaves: x\"");
			int numberOfLeaves = atoi(tokens[1].c_str());

			while(!inFile.eof() && (s.find("Size of the Tree") == s.npos))
			{
				s = IO::ReadLine(inFile);
			}

			Corpus::Parser::Tokenize(s,tokens,":");
			if(tokens.size() != 2)
				throw Corpus::tagexception("CLMTree::CreateTree, expected line \"Size of the Tree: x\"");
			int numberOfNodes = atoi(tokens[1].c_str());

			if((size_t)numberOfNodes != treeStrings.size())
				throw Corpus::tagexception("CLMTree::CreateTree, real read tree's size differs from stated size");

			std::vector< std::vector<std::string> > leafModels;

			for(;!inFile.eof() && (s.find("LM_") == s.npos); s = IO::ReadLine(inFile))
				;
			for(int lm = 1; lm <= numberOfLeaves; lm++)
			{
				std::vector<std::string> leafModel;
				s.clear();
				for(;!inFile.eof() && (s.find("LM_") == s.npos); s = IO::ReadLine(inFile))
				{
					leafModel.push_back(s);
				}
				leafModels.push_back(leafModel);
			}
			inFile.close();

			return new CLMTree(treeStrings,leafModels);
		}


		Corpus::CToken* CLMTree::Unify(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& srcSentence, TaKIPI::CLayers& layers)
		{
			return root->Unify(pattern,srcSentence,layers);
		}


	}// namespace Classifiers
}//namespace TaKIPI
