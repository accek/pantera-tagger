#include "../takipi_headers.h"
#include "Tagger.h"

#include "Config.h"
#include "IO.h"

#include "Classifiers/Classifier.h"
#include "Classifiers/BagClassifier.h"
#include "Classifiers/ClassifierRule.h"
#include "Classifiers/DecisionTree.h"
#include "Classifiers/DecisionTreeNode.h"
#include "Classifiers/LMTree.h"
#include "Classifiers/LMTreeNode.h"
#include "Classifiers/LogisticModel.h"
#include "Classifiers/NegRuleDecisionTree.h"
#include "Classifiers/NegRuleDecisionTreeNode.h"
#include "Classifiers/Ripper.h"
#include "Classifiers/RuleClassifier.h"
#include "Classifiers/VotingAverageClassifier.h"
#include "Classifiers/VotingWTAClassifier.h"
#include "Classifiers/Part.h"

namespace TaKIPI
{

	CRTTTagger::CRTTTagger(TaKIPI::CLayers& layers, int iterations, double cutOffLevel, bool dumpRules)
		:layers(layers), iterations(iterations), pGen(CPermutationGenerator(layers)), dumpRuleInfo(dumpRules)
	{
		randstate = 19;
		
		// initalize collections
		negPatterns = new std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>();
		patterns = new std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>();
		trees = new std::map<Corpus::CValue,Classifiers::CClassifier*>();
		negTrees = new std::map<Corpus::CValue,Classifiers::CNegRuleDecisionTree*>();

		std::vector<JOSKIPI::Rules::CRule*> *rules_vector = new std::vector<JOSKIPI::Rules::CRule*>();
		rules = new JOSKIPI::Rules::CRuleSet(rules_vector);
		dictionary = NULL;
		cutOff = new CCutOff(cutOffLevel);
	}

	CRTTTagger::~CRTTTagger(void)
	{
		if(rules)
		{
			delete rules;
			rules = NULL;
		}
		if(dictionary)
			delete dictionary;
		delete cutOff;

		if(trees)
		{
			for(std::map<Corpus::CValue,Classifiers::CClassifier*>::iterator it = trees->begin(); 
				it != trees->end();it++)
			{
				if(it->second)
					delete it->second;
			}
			delete trees;
			trees = NULL;
		}

		if(negTrees)
		{
			for(std::map<Corpus::CValue,Classifiers::CNegRuleDecisionTree*>::iterator it = negTrees->begin();
				it != negTrees->end(); it++)
			{
				if(it->second)
					delete it->second;
			}
			delete negTrees;
			negTrees = NULL;
		}

		if(patterns)
		{	// TODO: repair JOSKIPI::Patterns::CPattern and restore memory freeing
// 			for(std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>::iterator it = patterns->begin();
// 				it != patterns->end(); it++)
// 			{
// 				if(it->second)
// 					delete it->second;
// 			}
			
			delete patterns;
			patterns = NULL;
		}

		if(negPatterns)
		{
			for(std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>::iterator it = negPatterns->begin(); 
				it != negPatterns->end(); it++)
			{
				if(it->second)
					delete it->second;
			}
			delete negPatterns;
			negPatterns = NULL;
		}
	}

	void CRTTTagger::InitFromFile(std::string fileName)
	{
		std::ifstream file;
		std::string filePath = IO::GetFullPath(CConfig::commonPath, fileName);
		file.open(filePath.c_str());
		if(!file)
		{
			throw Corpus::tagexception("CRTTTagger::InitFromFile: could not open pattern file");
		}
		
		JOSKIPI::Parsers::CAntlrParser parser;
		try
		{
			parser.ParsePatterns(file, patterns);
		}
		catch(antlr::ANTLRException &e)
		{
			std::string msg("Error while parsing ");
			msg.append(fileName);
			msg.append("\n");
			msg.append(e.toString());
			msg.append("\n");
			throw Corpus::tagexception(msg.c_str());
		}
	}

	void CRTTTagger::TempInit()
	{
		std::ifstream rulFile;
		rulFile.open(IO::GetFullPath(CConfig::commonPath, CConfig::generalRuleFileName).c_str());
		if(!rulFile)
		{
			throw Corpus::tagexception("CRTTTagger::TempInit: could not open rule file");
		}
		JOSKIPI::Parsers::CAntlrParser parser(dumpRuleInfo);
		try
		{
			parser.ParseRules(rulFile, rules);
		}
		catch(antlr::ANTLRException &e)
		{
			std::string msg("Error while parsing ");
			msg.append(CConfig::generalRuleFileName);
			msg.append("\n");
			msg.append(e.toString());
			msg.append("\n");
			throw Corpus::tagexception(msg.c_str());
		}
	}

	JOSKIPI::Rules::CRuleSet* CRTTTagger::GetRules()
	{
		return rules;
	}

	void CRTTTagger::Load(bool loadWordDictionary, bool loadUDictionary)
	{
		LoadDictionary(loadWordDictionary, loadUDictionary);
		//DONE: InitFromFile takes relative path (relative to CConfig::commonPath)
		InitFromFile("patterns.pat");
		InitFromFile("patterns-cas.pat");
		InitFromFile("patterns-nmb-gnd.pat");
		TempInit();
		if(CConfig::classifierType == CConfig::Bag)
		{
			LoadAllBagged(Bag);
		}
		else if(CConfig::classifierType == CConfig::VotingWTA)
		{
			LoadAllBagged(VotingWTA);
		}
		else if(CConfig::classifierType == CConfig::VotingAverage)
		{
			LoadAllBagged(VotingAverage);
		}
		else
		{
			LoadTrees();
			if(CConfig::useNegRules)
				LoadNegTrees();
			if(CConfig::classifierType == CConfig::LMT)
				LoadLMTrees();
			if(CConfig::classifierType == CConfig::Ripper)
				LoadRipperRules();
			if(CConfig::classifierType == CConfig::PART)
				LoadPartRules();
		}
	}

	void CRTTTagger::LoadAllBagged(LoadMode loadMode)
	{
		LoadTrees(loadMode);
		LoadLMTrees(loadMode);
		LoadRipperRules(loadMode);
		LoadPartRules(loadMode);
	}

	void CRTTTagger::LoadDictionary(bool loadWordDictionary, bool loadUDictionary)
	{
		std::ifstream file,file2;

		if(loadWordDictionary)
		{
			std::cout << "Deserializacja slownika slow.";
			std::string path = IO::GetFullPath(CConfig::commonPath, CConfig::wordDictFileName);
			file.open(path.c_str(),std::ios::binary); //DONE: extracted to CCOnfig
			if(!file.is_open())
			{
				std::stringstream msg;
				msg << "BLAD: Brak pliku: " << path;
				throw Corpus::tagexception(msg.str().c_str());
			}

			Corpus::CWordDictionary::Deserialize(file);
			file.close();
		}

		if(loadUDictionary)
		{
			std::cout << "Deserializacja slownika unigramowego.";
	
			std::string path2 = IO::GetFullPath(CConfig::commonPath, CConfig::uDictFileName);
			file2.open(path2.c_str(),std::ios::binary);
			if(!file2.is_open())
			{
				throw Corpus::tagexception( (std::string("BLAD: Brak pliku: ") + path2).c_str() );
				
				exit(0);
			}
			dictionary = CUDictionary::Deserialize(file2);
			file2.close();
		}
		else
		{
			if(dictionary == NULL)
			{
				dictionary = new CUDictionary();
			}
		}
	}

	void CRTTTagger::LoadTrees(LoadMode loadMode)
	{
		using namespace Classifiers;

		std::cout << "Rozpoczynam ladowanie drzew c45.";

		std::string fileName;
		Corpus::CValue p;

		int step = 12;

		std::string treesDir = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir);

		while(IO::NextTreeFile(&fileName, &p, false, treesDir))
		{
			std::string path = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir, fileName);
			std::ifstream file;
			file.open(path.c_str(),std::ios::binary);
			if(!file)
			{
				std::stringstream msg;
				msg << "LoadTrees: nie mozna otworzyc pliku "
					<< path;

				throw Corpus::tagexception(msg.str().c_str());
			}

			CClassifier* tree = CDecisionTree::Deserialize(file);
			if(loadMode == Bag)
			{
				trees->insert(std::pair<Corpus::CValue,CClassifier*>(p,new CBagClassifier(tree,(*patterns)[p])));
			}
			else if(loadMode == VotingWTA)
			{
				trees->insert(std::pair<Corpus::CValue,CClassifier*>(p,new CVotingWTAClassifier(tree,(*patterns)[p])));
			}
			else if(loadMode == VotingAverage)
			{
				trees->insert(std::pair<Corpus::CValue,CClassifier*>(p,new CVotingAverageClassifier(tree,(*patterns)[p])));
			}
			else
			{
				trees->insert(std::pair<Corpus::CValue,CClassifier*>(p,tree));
			}

			file.close();
			if (--step == 0)
			{
				std::cout << ".";
				step = 12;
			}
		}
		std::cout << "OK (" << trees->size() << " drzew)\n";
	}

	void CRTTTagger::LoadNegTrees()
	{
		using Classifiers::CNegRuleDecisionTree;
		using Corpus::CValue;
		JOSKIPI::Parsers::CAntlrParser parser;

		std::ifstream file;
		std::string negPatFileName("negPatterns.pat");//TODO: extract to config!
		std::string negPatPath = IO::GetFullPath(CConfig::commonPath, negPatFileName);
		file.open(negPatPath.c_str());
		if(!file.is_open())
		{
			std::cerr << "Nie wykryto pliku "
				<< negPatPath
				<< " - drzewa negatywne wylaczone." << std::endl;	
			return;
		}
		try
		{
			parser.ParsePatterns(file, negPatterns);
		}
		catch(antlr::ANTLRException &e)
		{
			std::string msg("Error while parsing ");
			msg.append(negPatFileName);
			msg.append("\n");
			msg.append(e.toString());
			msg.append("\n");
			throw Corpus::tagexception(msg.c_str());
		}
		file.close();

		/* DONE: absolute paths
		if (!IO::ChangeDir(".\\NegTrees\\"))
			throw Corpus::tagexception("There is no \"NegTrees\" directory!");
		*/

		std::cout << "Rozpoczynam ladowanie drzew negatywnych.";

		std::string fileName;
		Corpus::CValue p;
		std::string negTreesDir = IO::GetFullPath(CConfig::commonPath, CConfig::negTreeSubDir);

		int step = 4;

		IO::SearchAgain();
		while(IO::NextTreeFile(&fileName, &p, false, negTreesDir))
		{
			std::string path = IO::GetFullPath(CConfig::commonPath, CConfig::negTreeSubDir, fileName);
			std::ifstream file;

			file.open(path.c_str(),std::ios::binary);
			CNegRuleDecisionTree* tree = CNegRuleDecisionTree::Deserialize(file);
			negTrees->insert(std::pair<CValue,CNegRuleDecisionTree*>(p,tree));

			//file.open(fileName.c_str());
			//CDecisionTree* tree = CDecisionTree::CreateTree(file,true,p.first,p.second);
			//tree->Serialize();
			//delete tree;

			file.close();
			if (--step == 0)
			{
				std::cout << ".";
				step = 4;
			}
		}
		std::cout << "OK\n";

		// IO::ChangeDir(".."); DONE: absolute paths
	}

	// �aduje drzewa LMT, wyrzucaj�c za�adowane klasyfikatory jesli loadMode jest Override, lub wrzucajac do CBagClassifier, jesli loadMode jest Bag
	void CRTTTagger::LoadLMTrees(LoadMode loadMode)
	{
		using namespace Classifiers;
		/* DONE: absolute path
		if (!IO::ChangeDir(".\\Trees\\"))
			throw Corpus::tagexception("There is no \"Trees\" directory!");
		*/

		std::cout << "Rozpoczynam ladowanie drzew LMT.";

		std::string fileName;
		Corpus::CValue p;

		std::string treesDir = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir);

		int step = 12;
		IO::SearchAgain();
		while(IO::NextLMTreeFile(&fileName, &p, treesDir))
		{
			std::string path = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir, fileName);
			std::ifstream file;

			//TODO: (de)serializacja lmt
			//file.open(fileName.c_str(),ios::binary);
			//CClassifier* tree = CDecisionTree::Deserialize(file);

			file.open(path.c_str());
			CLMTree* tree = CLMTree::CreateTree(file);

			//tree->Serialize();
			//delete tree;
			if(tree != NULL)
			{
				CClassifier *oldTree = (*trees)[p];
				if(loadMode == Override)
				{
					if(oldTree != NULL)
					{
						delete oldTree;
					}
					(*trees)[p] = tree; 
				}
				else//simple bag or voting
				{
					if(oldTree == NULL)
					{
						switch(loadMode)
						{
						case Bag:
							(*trees)[p] = new CBagClassifier(tree,(*patterns)[p]);
							break;
						case VotingWTA:
							(*trees)[p] = new CVotingWTAClassifier(tree,(*patterns)[p]);
							break;
						case VotingAverage:
							(*trees)[p] = new CVotingAverageClassifier(tree,(*patterns)[p]);
							break;
						}
					}
					else
					{
						((CBagClassifier*)oldTree)->Add(tree,(*patterns)[p]);
					}

				}
			}
			//oryginalne podstawianie drzew
			//pair < map<CValue,CClassifier*>::iterator, bool > pr;
			//pr = trees.insert(pair<CValue,CClassifier*>(p,tree));
			//if(!pr.second)
			//{
			//	delete pr.first->second;
			//	trees.erase(pr.first);
			//	trees.insert(pair<CValue,CClassifier*>(p,tree));
			//}

			//tree->Serialize();
			//delete tree;

			file.close();
			if (--step == 0)
			{
				std::cout << ".";
				step = 12;
			}
		}
		std::cout << "OK" <<std::endl;

		// IO::ChangeDir(".."); DONE: absolute path
	}

	//�aduje zestawy regu� Rippera wyrzucaj�c to, co ju� ewentualnie jest
	void CRTTTagger::LoadRipperRules(LoadMode loadMode)
	{
		using Corpus::CValue;
		using namespace Classifiers;

		/*
		if (!IO::ChangeDir(".\\Trees\\"))//DONE: absolute path
			throw Corpus::tagexception("There is no \"Trees\" directory!");
		*/
		std::cout << "Rozpoczynam ladowanie regul Rippera.";

		std::string fileName;
		CValue p;
		std::string treesDir = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir);

		int step = 12;
		IO::SearchAgain();
		while(IO::NextRipperFile(&fileName, &p, treesDir))
		{
			std::string path = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir, fileName);
			std::ifstream file;
			file.open(path.c_str());


			CRipper * ripper = CRipper::CreateClassifier(file,p.first,p.second);
			if(ripper != NULL)
			{
				CClassifier *oldClass = (*trees)[p];
				if(loadMode == Override)
				{
					if(oldClass != NULL)
					{
						delete oldClass;
					}
					(*trees)[p] = ripper;				
				}
				else
				{
					if(oldClass == NULL)
					{
						switch(loadMode)
						{
						case Bag:
							(*trees)[p] = new CBagClassifier(ripper,(*patterns)[p]);
							break;
						case VotingWTA:
							(*trees)[p] = new CVotingWTAClassifier(ripper,(*patterns)[p]);
							break;
						case VotingAverage:
							(*trees)[p] = new CVotingAverageClassifier(ripper,(*patterns)[p]);
							break;
						}
					}
					else
					{
						((CBagClassifier*)oldClass)->Add(ripper,(*patterns)[p]);
					}
				}
			}
			else
			{
				//co robic, jak jest NULL? moze jakas regula domyslna albo jednak wywalic?
				std::cerr << std::endl << path << " - nie udalo sie wczytac regul Rippera." << std::endl;
				if((*trees)[p] == NULL)
				{
					throw Corpus::tagexception(path.append(" - nie udalo wczytac sie regul Rippera, a brak zamiennika c4.5!").c_str());
				}
			}
			file.close();
			if (--step == 0)
			{
				std::cout << ".";
				step = 12;
			}
		}
		std::cout << "OK" << std::endl;

		// IO::ChangeDir(".."); DONE: absolute path
	}

	//�aduje zestawy regu� PART wyrzucaj�c to, co ju� ewentualnie jest
	void CRTTTagger::LoadPartRules(LoadMode loadMode)
	{
		using Corpus::CValue;
		using namespace Classifiers;

		/*
		if (!IO::ChangeDir(".\\Trees\\"))//DONE: absolute path
			throw Corpus::tagexception("There is no \"Trees\" directory!");
		*/

		std::cout << "Rozpoczynam ladowanie regul PART.";

		std::string fileName;
		CValue p;

		int step = 12;
		std::string treesDir = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir);
		IO::SearchAgain();
		while(IO::NextPartFile(&fileName, &p, treesDir))
		{
			std::string path = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir, fileName);
			std::ifstream file;

			file.open(path.c_str());

			CPart * Part = CPart::CreateClassifier(file,p.first,p.second);
			if(Part != NULL)
			{
				CClassifier *oldClass = (*trees)[p];
				if(loadMode == Override)
				{
					if(oldClass != NULL)
					{
						delete oldClass;
					}
					(*trees)[p] = Part;
				}
				else
				{
					if(oldClass == NULL)
					{
						switch(loadMode)
						{
						case Bag:
							(*trees)[p] = new CBagClassifier(Part,(*patterns)[p]);
							break;
						case VotingWTA:
							(*trees)[p] = new CVotingWTAClassifier(Part,(*patterns)[p]);
							break;
						case VotingAverage:
							(*trees)[p] = new CVotingAverageClassifier(Part,(*patterns)[p]);
							break;
						}
					}
					else
					{
						((CBagClassifier*)oldClass)->Add(Part,(*patterns)[p]);
					}
				}
			}
			else
			{
				//co robic, jak jest NULL? moze jakas regula domyslna albo jednak wywalic?
				std::cerr << std::endl << path << " - nie udalo sie wczytac regul Parta." << std::endl;
				if((*trees)[p] == NULL)
				{
					throw Corpus::tagexception(path.append(" - nie udalo wczytac sie regul PART, a brak zamiennika c4.5!").c_str());
				}
			}
			file.close();
			if (--step == 0)
			{
				std::cout << ".";
				step = 12;
			}
		}
		std::cout << "OK" << std::endl;

		// IO::ChangeDir(".."); DONE: absolute path
	}

	bool CRTTTagger::Iteration(Corpus::CSentence& sentence,	
		std::map<int,std::vector<std::pair<JOSKIPI::Patterns::CPattern*,Classifiers::CClassifier*> > >& chosenTrees)
	{
		if(!pGen.FirstPermutation())
			return false;

		do
		{
			ProceedSinglePermutation(sentence,chosenTrees);
		}
		while(pGen.NextPermutation());

		sentence.Goto(Corpus::CPosition::nowhere);
		if (!pGen.MoveToNextAmbToken())
			return false;
		else
			return true;
	}

	bool CRTTTagger::ProceedSinglePermutation(Corpus::CSentence& sentence,
		std::map<int,std::vector<std::pair<JOSKIPI::Patterns::CPattern*,Classifiers::CClassifier*> > >& chosenTrees)
	{
		if (!pGen.MoveToNextAmbToken())
			return false;

		do
		{
			std::vector<std::pair<JOSKIPI::Patterns::CPattern*,Classifiers::CClassifier*> >::iterator it 
				= chosenTrees[sentence.GetPosition()].begin();

			std::vector<Corpus::CToken*> probTokens;

			for(; it != chosenTrees[sentence.GetPosition()].end(); it++)
			{
				probTokens.push_back(it->second->Unify(*it->first,sentence,layers));
			}

#ifdef OCR
			for(int i = 0; i < (int)probTokens.size(); ++i)
			{
				if(probTokens[i] != NULL)
					for(int j = 0; j < (int)probTokens[i]->GetLexCount(); ++j)
					{
						sentence.GetCurrentToken()->GetLexem(j)->bufferCD += probTokens[i]->GetLexem(j)->probability
							- ((CDecisionTree*)chosenTrees[sentence.GetPosition()][i].second)->meanProb;
					}
			}
#endif 

			AverageClassifier(probTokens,&sentence);

		}
		while(pGen.MoveToNextAmbToken());

		return true;
	}

	void CRTTTagger::AverageClassifier(std::vector<Corpus::CToken*> probTokens, Corpus::CSentence* sentence)
	{
		Corpus::CToken* average = sentence->GetCurrentToken()->CreateHardCopy();

		bool firstPass = true;
		bool wasChange = false;
		int counter = 0;

		for(int iT = 0; iT < (int)probTokens.size(); iT++)
		{
			if(probTokens[iT] == NULL)
				continue;

			counter++;

			if(firstPass)
			{
				for(int i = 0; i < (int) average->GetLexCount(); i++)
					average->GetLexem(i)->probability = probTokens[iT]->GetLexem(i)->probability;
				firstPass = false;
			}
			else
			{
				for(int i = 0; i < (int)average->GetLexCount(); i++)
					average->GetLexem(i)->probability += probTokens[iT]->GetLexem(i)->probability;
			}

			wasChange = true;

			Corpus::CToken* t = probTokens[iT];
			delete t;
		}

		if(wasChange)
		{
			for(int j = 0; j < (int)average->GetLexCount(); j++)
			{
				sentence->GetCurrentToken()->SetLexProbability(j,average->GetLexem(j)->probability/counter);
			}
		}

		delete average;
	}

	void CRTTTagger::NormalizeProbabilities(Corpus::CSentence& sentence)
	{
		for(int i = 0 ; i < sentence.Size(); i++)
			NormalizeProbabilities(*sentence[i]);
	}

	void CRTTTagger::NormalizeProbabilities(Corpus::CToken& t)
	{
		long double sum = exp((long double)t.GetLexem(0)->probability);

		//Obliczamy sum� wszystkich lexem�w.
		for(int k = 1; k < (int)t.GetLexCount(); k++)
		{
			sum += exp((long double)t.GetLexem(k)->probability);
		}

		sum = log(sum);

		for(int j = 0; j < (int)t.GetLexCount(); j++)
		{
			t.SetLexProbability(j,-sum);
		}
	}

	void CRTTTagger::SetStartProbabilities(Corpus::CSentence& sentence)
	{
		if(!sentence.FirstPosition())
			return;

		double newProb, lambda, denominator;
		for(int i = 0; i < sentence.Size(); i++)
		{
			std::string s = sentence[i]->GetOrth();
			// GetToken zamienia na ma�e litery przed pobraniem ze s�ownika
			Corpus::CToken* token = dictionary->GetToken(sentence[i]);

			// Je�li forma taka wyst�puje w s�owniku unigramowym...
			if (token != 0)
			{
				// Obliczamy sum� liczno�ci wszystkich lexem�w 
				// (przechowywan� w s�owniku unigramowym w nieu�ywanym probability).
				double sum = 0;
				for(int k = 0; k < (int)token->GetLexCount(); k++)
				{
					sum += token->GetLexem(k)->probability;
				}

				// Obliczamy wsp��czynnik lambda [LAMBDA = (K-1)/K] potrzebny do wyg�adzania.
				lambda = ((double)(token->GetLexCount() - 1))/((double)token->GetLexCount());
				// Mianownik wyg�adzanej warto�ci prawdopodobie�stwa.
				denominator = sum + (double)(token->GetLexCount() - 1);

				for(int k = 0; k < (int)sentence[i]->GetLexCount(); k++)
				{
					for(int l = 0; l < (int)token->GetLexCount(); l++)
					{
						// Je�eli si� zgadzaj� lexemy w tym co mamy i tym co otrzymali�my ze s�ownika unigramowego...
						if(*sentence[i]->GetLexem(k) == *token->GetLexem(l))
						{
							// Logarytmiczna, wyg�adzona warto�� s�ownika unigramowego.
							newProb = log((token->GetLexem(l)->probability + lambda)/denominator);
							sentence[i]->SetLexProbability(k,newProb);
						}
					}
				}

				// Je�li kt�ry� lexem nie wyst�pi� w s�owniku dostaje minimalne wyg�adzone prawdopodobie�stwo.
				for(int k = 0; k < (int)sentence[i]->GetLexCount(); k++)
				{
					if(sentence[i]->GetLexem(k)->probability == 0)
					{
						newProb = log(lambda/denominator);
						sentence[i]->SetLexProbability(k,newProb);
					}
				}
			}
			// Je�li forma nie wyst�puj�, to wszystkie lexemy dostaj� te same prawdopodobie�stwo (sumowalne do 1)(logarytmiczne).
			else
			{
				newProb = -log((double)sentence[i]->GetLexCount());
				for(int k = 0; k < (int)sentence[i]->GetLexCount(); k++)
				{
					sentence[i]->GetLexem(k)->probability = newProb;
				}
			}

#ifdef OCR
			double sumProbs = 0;
			for(int lexemIter = 0; lexemIter < (int)sentence[i]->GetLexCount(); ++lexemIter)
			{
				sumProbs += exp(sentence[i]->GetLexem(lexemIter)->probability);
			}
			double prob = sumProbs/(int)sentence[i]->GetLexCount();
			for(int lexemIter = 0; lexemIter < (int)sentence[i]->GetLexCount(); ++lexemIter)
			{
				sentence[i]->GetLexem(lexemIter)->bufferCD = 0;
				sentence[i]->GetLexem(lexemIter)->CD = exp(sentence[i]->GetLexem(lexemIter)->probability) - prob;
				sentence[i]->GetLexem(lexemIter)->lastCD = sentence[i]->GetLexem(lexemIter)->CD;
			}
#endif

			if(CConfig::useNegRules)
			{
				std::map<Corpus::CValue,Classifiers::CNegRuleDecisionTree*>::iterator tIter;

				Corpus::CToken* t = sentence.GetCurrentToken();
				for(tIter = negTrees->begin(); tIter != negTrees->end(); tIter ++)
				{
					if((*t)[tIter->first] == tIter->first)
					{
						tIter->second->UseNegRuleTree(* (*negPatterns)[tIter->first],sentence);
					}
				}
				sentence.NextPosition();
			}
		}
	}

	void CRTTTagger::SeleceDecisionTrees(std::map<int,std::vector<std::pair
		<JOSKIPI::Patterns::CPattern*,Classifiers::CClassifier*> > >& chosenTrees, Corpus::CSentence& sentence)
	{
		Corpus::CValue p,mask;
		layers.GetLayerMask(mask);
		sentence.FirstPosition();

		do
		{
			if(!pGen.IsAmb(sentence.GetPosition()))
				continue;

			p = (*sentence.GetCurrentToken())[mask];
			(*sentence.GetCurrentToken()).ambClass = p;

			//--------------------------------------------------------------------

			std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>::iterator pIter;

			int bitNumber = INT_MAX;

			for(pIter = patterns->begin(); pIter != patterns->end(); pIter ++)
			{
				if ( (p.first & pIter->first.first) == p.first 
					&& (p.second & pIter->first.second) == p.second) 
				{
					if (Corpus::Parser::BitCount(pIter->first.first) 
						+ Corpus::Parser::BitCount(pIter->first.second) < bitNumber)
					{
						bitNumber = Corpus::Parser::BitCount(pIter->first.first) 
							+ Corpus::Parser::BitCount(pIter->first.second);
					}
				}
			}

			//--------------------------------------------------------------------
			// ALGORYTM KUBE�K�W (POZIOM IDENTYFIKACJI)

			//bool isContained = false;

			for(std::map<Corpus::CValue,Classifiers::CClassifier*>::iterator iter = trees->begin();
				iter != trees->end(); iter++)
			{
				if ((p.first & iter->first.first) == p.first
					&& (p.second & iter->first.second) == p.second)
				{
					int bits = Corpus::Parser::BitCount(iter->first.first) 
						+ Corpus::Parser::BitCount(iter->first.second);
					if ( bits <= bitNumber)
					{
						if(chosenTrees.find(sentence.GetPosition()) == chosenTrees.end())
						{
							std::vector<std::pair<JOSKIPI::Patterns::CPattern*,Classifiers::CClassifier*> > v;
							v.push_back(std::pair<JOSKIPI::Patterns::CPattern*,Classifiers::CClassifier*>
								( (*patterns)[iter->first],iter->second));
							chosenTrees.insert(std::pair<int,std::vector<std::pair<JOSKIPI::Patterns::CPattern*,
								Classifiers::CClassifier*> > >(sentence.GetPosition(),v));
							//break;
						}
						else
							chosenTrees[sentence.GetPosition()].push_back(std::pair<JOSKIPI::Patterns::CPattern*,
							Classifiers::CClassifier*>((*patterns)[iter->first],iter->second));
					}
				}
			}

			//--------------------------------------------------------------------
		}
		while(sentence.NextPosition());
	}


	/*
	G��wna metoda tagowania. Dostaje zdanie na kt�rym dzia�a i z kt�rego wycina odrzucone lexemy.

	*/
	void CRTTTagger::Disambiguate(Corpus::CSentence &sentence, bool filtered, bool withPermutations, bool useRules)
	{
		pGen.SetSentence(&sentence,0);

		// Ustawiamy si� na pierwszej warstwie.
		if (!pGen.FirstLayer(withPermutations))
			throw Corpus::tagexception("There are no layers to disambiguate.");

		SetStartProbabilities(sentence);

		// Zmiana: stosujemy regu�y dop�ki co� si� dzieje
		if(useRules && CConfig::rulesUseNum > 0)
		{
			bool anyChange;
			do
			{
				sentence.FirstPosition();
				anyChange = false;
				do
				{
					if((int)sentence.GetCurrentToken()->GetLexCount() > 1)
						anyChange = anyChange || rules->Apply(sentence);
				}
				while(sentence.NextPosition());
			}
			while(anyChange);
		}

#ifdef OCR
		sentence.FirstPosition();
		do
		{
			CToken* t = sentence.GetCurrentToken();
			if((int)t->GetLexCount() == 1)
			{
				t->GetLexem(0)->CD = 4;
				t->GetLexem(0)->lastCD = 0;
				t->GetLexem(0)->bufferCD = 0;
			}
		}
		while(sentence.NextPosition());
#endif

		// G��wna p�tla dzia�ania taggera.
		do
		{
			std::map<int,std::vector<std::pair<JOSKIPI::Patterns::CPattern*,Classifiers::CClassifier*> > > chosenTrees;
			SeleceDecisionTrees(chosenTrees,sentence);
			for(int i = 0; i < iterations; i++)
			{
				// Je�li wszystkie wyrazy s� zidentyfikowane to przerywamy.
				if(withPermutations)
				{
					if(!Iteration(sentence,chosenTrees))
						i = iterations;
				}
				else
				{
					if(!ProceedSinglePermutation(sentence,chosenTrees))
						i = iterations;
				}
				NormalizeProbabilities(sentence);
				if(filtered)
				{
					cutOff->CutOff(sentence);
					NormalizeProbabilities(sentence);
				}
				//
			}
			if(filtered)
			{
				cutOff->LeaveBestPackage(sentence,layers);
				NormalizeProbabilities(sentence);
			}

#ifdef OCR
			for(int sIter = 0; sIter < sentence.Size(); sIter++)
			{
				CToken* tempTok = sentence.GetToken(sIter);

				//if(chosenTrees.find(sIter) == chosenTrees.end())
				//{
				//	continue;
				//}

				for(int tIter = 0; tIter < (int)tempTok->GetLexCount(); ++tIter)
				{
					CLexem* tempLex = tempTok->GetLexem(tIter);

					if(tempLex->bufferCD != 0)
					{
						tempLex->CD += tempLex->bufferCD;
						tempLex->lastCD = tempLex->bufferCD;
					}
					else
						tempLex->CD += tempLex->lastCD;
					tempLex->bufferCD = 0;
				}
			}
#endif

		}
		while(pGen.NewLayer(withPermutations));
	}

	void CRTTTagger::TagSentence(Corpus::CSentence *tagdSent, Corpus::CSentence *sourceDisdSent, bool forceOne)
	{
		using namespace Corpus;
		if(tagdSent->Size() != sourceDisdSent->Size())
			throw tagexception("Tagged sentence and sourceDisdSent differ in size.");

		// foreach tok in tagdSent
		for(int i = 0; i < (int)tagdSent->Size(); i++)
		{
			int best = 0;
			int disambs = 0;
			for(int k = 1; k < (int)(*sourceDisdSent)[i]->GetLexCount(); k++)
			{
				if((*sourceDisdSent)[i]->GetLexem(k)->probability > (*sourceDisdSent)[i]->GetLexem(best)->probability)
					best = k;
			} // best := max-prob lexem of dis'd token

			TFlexClass bestFlex = (*sourceDisdSent)[i]->GetLexem(best)->GetTag().FlexClass;
			DLONG bestGCat = (((*sourceDisdSent)[i]->GetLexem(best)->GetTag().Categories) & (gcm_liczba|gcm_rodzaj|gcm_przypadek));
			for(int j = 0; j < (int)(*tagdSent)[i]->GetLexCount(); j++)
			{ // foreach tag'd lexem
				CLexem* l = (*tagdSent)[i]->GetLexem(j);

				if(l->GetTag().FlexClass == bestFlex
					&& (l->GetTag().Categories & (gcm_liczba|gcm_rodzaj|gcm_przypadek)) == bestGCat)
				{
					++disambs;
					l->SetDisamb(true);
				}
				else
					l->SetDisamb(false);
				
				l->PostprocessSubstGer();
			}
			
			if(forceOne && (disambs > 0))
			{
				ForceOneDisamb((*tagdSent)[i]);
			}
		} // foreach tok
	}
	
	void CRTTTagger::ForceOneDisamb(Corpus::CToken *tagdTok)
	{
		// gather disamb lexem indices
		std::vector<int> dis_lex_nums;
		for(int lex_num = 0; lex_num < (int)tagdTok->GetLexCount(); ++lex_num)
		{
			if(tagdTok->GetLexem(lex_num)->GetDisamb())
			{
				dis_lex_nums.push_back(lex_num);
			}
		}
		int disambs = (int)dis_lex_nums.size();
		if(disambs > 1)
		{
			// select pseudo-random one
			int selected = RandomR() % disambs;
			for(int dis_num = 0; dis_num < disambs; ++dis_num)
			{
				// switch this one on iff its the selected one
				tagdTok->GetLexem(dis_lex_nums[dis_num])->SetDisamb(dis_num == selected);
			}
		}
	}
	
	unsigned int CRTTTagger::RandomR()
	{
		// RAND_MAX assumed to be 32767
		randstate = randstate * 1103515245 + 12345;
		return((unsigned int)(randstate/65536) % 32768);
	}

	void CRTTTagger::RunKoder()
	{
		std::cout << "KODER: Przetwarzanie drzew\n";
		std::string fileName;
		Corpus::CValue p;
		int step = 12;

		std::string treesDir = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir);
		while(IO::NextTreeFile(&fileName, &p, true, treesDir))
		{
			std::string path = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir, fileName);
			std::ifstream file;
			
			//file.open(fileName.c_str(),ios::binary);
			//CDecisionTree* tree = CDecisionTree::Deserialize(file);
			//trees.insert(pair<CValue,CDecisionTree*>(p,tree));
			// otwieramy drzewa w postaci tekstowej (pliki wygenerowane przez c4.5)

			//std::cout << "Koding file " << path << "\n"; // TODO
			file.open(path.c_str());
			if(!file.is_open())
			{
				std::stringstream msg;
				msg << "Koder: nie mozna otworzyc pliku "
					<< path;

				throw new Corpus::tagexception(msg.str().c_str());
			}
			Classifiers::CDecisionTree* tree = Classifiers::CDecisionTree::CreateTree(file,true,p.first,p.second);
			if(tree != NULL)
			{
				tree->Serialize();
				delete tree;
				
				if (--step == 0)
				{
					std::cout << ".";
					step = 12;
				}
			}
			file.close();
		}
		std::cout << " OK\n";
	} // void CRTTTagger::RunKoder

	void CRTTTagger::ConvertDicts(bool toTxt)
	{
		std::ifstream file,file2;
		static std::string wtxtpath("WDict.txt");
		static std::string utxtpath("UDict.txt");
		TaKIPI::CUDictionary *udict = NULL;

		if(toTxt)
		{
			std::ifstream inwfile;
			std::cout << "Reading word dict (bin)\n";
			std::string wpath = IO::GetFullPath(CConfig::commonPath, CConfig::wordDictFileName);
			inwfile.open(wpath.c_str(),std::ios::binary);
			if(!inwfile)
			{
				std::string msg("ERROR: cannot open dict file: "); msg.append(wpath);
				throw Corpus::tagexception(msg.c_str());
			}
			Corpus::CWordDictionary::Deserialize(inwfile);
			inwfile.close();

			std::cout << "Writing word dict (txt)\n";
			std::ofstream outwfile(wtxtpath.c_str()); // text mode
			if(!outwfile)
			{
				std::string msg("ERROR: cannot open dict out file: ");
				msg.append(wtxtpath);
				throw Corpus::tagexception(msg.c_str());
			}
			Corpus::CWordDictionary::GetInstance()->SerializeAsText(outwfile);
			outwfile.close();

			std::cout << "Reading uni dict (bin)\n";
			std::string upath = IO::GetFullPath(CConfig::commonPath, CConfig::uDictFileName);
			std::ifstream inufile(upath.c_str(),std::ios::binary);
			if(!inufile)
			{
				std::string msg("ERROR: cannot open dict file: "); msg.append(upath);
				throw Corpus::tagexception(msg.c_str());
			}
			udict = TaKIPI::CUDictionary::Deserialize(inufile);
			inufile.close();
			std::cout << "Writing uni dict (txt)\n";
			std::ofstream outufile(utxtpath.c_str()); // text mode
			if(!outufile)
			{
				std::string msg("ERROR: cannot open dict out file: ");
				msg.append(utxtpath);
				throw Corpus::tagexception(msg.c_str());
			}
			udict->SerializeAsText(outufile);
			delete udict;
			outufile.close();
		}
		else // text -> bin
		{
			std::ifstream inwfile;
			std::cout << "Reading word dict (txt)\n";
			inwfile.open(wtxtpath.c_str()); // text mode
			if(!inwfile)
			{
				std::string msg("ERROR: cannot open dict textfile: "); msg.append(wtxtpath);
				throw Corpus::tagexception(msg.c_str());
			}
			Corpus::CWordDictionary::DeserializeFromText(inwfile);
			inwfile.close();

			std::cout << "Writing word dict (bin)\n";
			std::string outwpath = IO::GetFullPath(CConfig::commonPath, CConfig::wordDictFileName);
			std::ofstream outwfile(outwpath.c_str(), std::ios::binary); // bin mode
			if(!outwfile)
			{
				std::string msg("ERROR: cannot open dict out file: ");
				msg.append(outwpath);
				throw Corpus::tagexception(msg.c_str());
			}
			Corpus::CWordDictionary::GetInstance()->Serialize(outwfile);
			outwfile.close();
			std::cout << "Word dict written to " << outwpath << "\n";

			std::cout << "Reading uni dict (txt)\n";
			std::ifstream inufile(utxtpath.c_str()); // text mode
			if(!inufile)
			{
				std::string msg("ERROR: cannot open dict textfile: "); msg.append(utxtpath);
				throw Corpus::tagexception(msg.c_str());
			}
			udict = TaKIPI::CUDictionary::DeserializeFromText(inufile);
			inufile.close();
			std::cout << "Writing uni dict (bin)\n";
			std::string outupath = IO::GetFullPath(CConfig::commonPath, CConfig::uDictFileName);
			std::ofstream outufile(outupath.c_str(), std::ios::binary); // bin mode
			if(!outufile)
			{
				std::string msg("ERROR: cannot open dict out file: ");
				msg.append(outupath);
				throw Corpus::tagexception(msg.c_str());
			}
			udict->Serialize(outufile);
			outufile.close();
			delete udict;
			std::cout << "Uni dict written to " << outupath << "\n";
		}
	} // ConvertDicts

	void CRTTTagger::GenerateDicts(std::string trainCorpusFile)
	{
		std::cerr << "Generating dictionaries...\n";
		// udajemy, że tworzymy normalny tager -- chodzi o to, by upewnić się, że w słowniku są te słowa
		// z reguł i wzorców, które normalnie by tam się znalazły
		std::vector<int> layers;
		std::vector<int> attrib;

		attrib.push_back(Corpus::CToken::flex);
		attrib.push_back(Corpus::CToken::nmb);
		attrib.push_back(Corpus::CToken::gnd);
		attrib.push_back(Corpus::CToken::cas);

		layers.push_back(0);
		layers.push_back(1);
		layers.push_back(3);

		TaKIPI::CLayers l(attrib,layers);

		CRTTTagger tagger(l,1,0.01);
		tagger.Load(false, false); // bez słowników
		
		TaKIPI::CUDictionary *udict = tagger.dictionary;
		Corpus::CWordDictionary *wdict = Corpus::CWordDictionary::GetInstance();
		
		std::ifstream co_input(trainCorpusFile.c_str());
		if(!co_input)
		{
			std::string msg("Cannot open training file ");
			msg.append(trainCorpusFile);
			throw Corpus::tagexception(msg.c_str());
		}
		// disambOnly = false (??), encodeToCp1250 = false (outEnc == inEnc == utf8)
		Corpus::Tokenisation::CCorpusTokenReader tokrdr(co_input, false, false);
		Corpus::CToken *tok;
		int toksSoFar = 0;
		while( (tok = tokrdr.GetNext()) != NULL )
		{
			if(tok->GetLexCount() > 1)
			{
				udict->Add(tok);
				if(++toksSoFar % 100000 == 0)
					std::cerr << toksSoFar << " tokens processed\n";
			}
			//Corpus::Tokenisation::CTokenReader::Free(tok);
			// TODO memory leak, handle it within udict::add?
		}
		std::cerr << toksSoFar << " tokens processed\n";
		
		std::cout << "Writing word dict (bin), " << wdict->Count() << " forms\n";
		std::string outwpath = IO::GetFullPath(CConfig::commonPath, CConfig::wordDictFileName);
		std::ofstream outwfile(outwpath.c_str(), std::ios::binary); // bin mode
		if(!outwfile)
		{
			std::string msg("ERROR: cannot open dict out file: "); msg.append(outwpath);
			throw Corpus::tagexception(msg.c_str());
		}
		Corpus::CWordDictionary::GetInstance()->Serialize(outwfile);
		outwfile.close();
		std::cout << "Word dict written to " << outwpath << "\n";

		std::cout << "Writing uni dict (bin)\n";
		std::string outupath = IO::GetFullPath(CConfig::commonPath, CConfig::uDictFileName);
		std::ofstream outufile(outupath.c_str(), std::ios::binary); // bin mode
		if(!outufile)
		{
			std::string msg("ERROR: cannot open dict out file: "); msg.append(outupath);
			throw Corpus::tagexception(msg.c_str());
		}
		udict->Serialize(outufile);
		outufile.close();
		std::cout << "Uni dict written to " << outupath << "\n";
	}

}//namespace TaKIPI


