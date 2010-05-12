
#include "../../takipi_headers.h"

#include "../Config.h"
#include "../IO.h"

#include "Teacher.h"
#include "Filter.h"

namespace TaKIPI
{
	namespace Learning
	{

		CTeacher::CTeacher(TaKIPI::CLayers layers, Corpus::Tokenisation::CTokenReader& corpora, int layer)	
			:layers(layers),reader(corpora), layer(layer)
		{
			patterns = new std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>();
			patternsOut = new std::map<Corpus::CValue,std::ofstream*>();

			sentencer = new Corpus::ExtSentencer::CSentencer(corpora);

			std::vector<JOSKIPI::Rules::CRule*> *rules_vector = new std::vector<JOSKIPI::Rules::CRule*>();
			rules = new JOSKIPI::Rules::CRuleSet(rules_vector);
			dump = 0;
			this->treeGenerationLevel = 0;

			std::vector<int>* layersV = new std::vector<int>();
			std::vector<int>* attrib = new std::vector<int>();

			switch(layer)
			{
			case 0:
				InitFromFile("patterns.pat");// DONE: relative path
				break;
			case 1:
				InitFromFile("patterns-nmb-gnd.pat");// DONE: relative path
				attrib->push_back(Corpus::CToken::flex);
				layersV->push_back(0);
				break;
			case 2:
				InitFromFile("patterns-cas.pat");// DONE: relative path
				attrib->push_back(Corpus::CToken::flex);
				layersV->push_back(0);
				attrib->push_back(Corpus::CToken::nmb);
				attrib->push_back(Corpus::CToken::gnd);
				layersV->push_back(1);
				break;
			default:
				throw Corpus::tagexception("Invalid layer number.");
			}

			TaKIPI::CLayers* l = new TaKIPI::CLayers(*attrib,*layersV);
			this->tagger = new CRTTTagger(*l,CConfig::iterations,CConfig::cutLevel);
			std::cout << "\tTagger obj created\n";
			this->tagger->Load(false);
			std::cout << "\tTagger obj: data loaded\n";
		}

		CTeacher::~CTeacher(void)
		{
			if(rules)
				delete rules;
			if(dump)
				delete dump;

			for(std::map<Corpus::CValue,std::ofstream*>::iterator i = patternsOut->begin(); 
				i != patternsOut->end(); i++)
			{
				std::ofstream* file = i->second;

				if(file->is_open())
					file->close();
				delete file;
			}

			delete tagger;
			delete sentencer;

			if(patterns)
			{
				delete patterns;
				patterns = NULL;
			}

			if(patternsOut)
			{
				delete patternsOut;
				patternsOut = NULL;
			}
		}


		Corpus::CToken::Attribute CTeacher::GetAttribute()
		{
			return (Corpus::CToken::Attribute)layers.GetCurrentAttribute();
		}

		bool CTeacher::FirstPosition()
		{
			sentence->Goto(-1);
			return sentence->NextPosition();
		}

		bool CTeacher::NextPosition()
		{
			while(sentence->NextPosition())
			{
				if (CFilter::IsAmb(*sentence,layers))
					return true;
			}
			return false;
		}

		bool CTeacher::ReadNextSentence()
		{
			//std::string s;
			sentence = sentencer->GetNext();
			if(sentence != NULL)
			{
				// teacher expects tokens in utf8, while ExtSentencer uses cp1250 internally (they come from TokenReader)
				sentence->ConvertEncoding(Corpus::Parser::cp1250, Corpus::Parser::utf8);;
				return true;
			}
			return false;
		}

		void CTeacher::TempInit()
		{
			IO::MakeSubdir(CConfig::ruleSubDir);
			JOSKIPI::Parsers::CAntlrParser parser;

			std::ifstream rulFile;
			rulFile.open(IO::GetFullPath(CConfig::commonPath, CConfig::generalRuleFileName).c_str());
			if(!rulFile)
			{
				std::stringstream msg;
				msg << "CTeacher::TempInit: could not open rule file: "
					<< IO::GetFullPath(CConfig::commonPath, CConfig::generalRuleFileName);
				throw Corpus::tagexception(msg.str().c_str());
			}
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

		void CTeacher::Learn(bool normalTrees, bool disambOnly, bool useRules)
		{
			using namespace Corpus;
			using namespace std;

			int i = 0;
			int notContained = 0;
			int disambCut = 0;
			int disambCutSaved = 0;
			int multipleRuleUse = 0; // TODO remove

			if(useRules)
				TempInit();

			while(ReadNextSentence())
			{
				CSentence* oryginal = sentence->CreateCopy();

				if (!sentence->FirstPosition())
					continue;

				// Zmiana: stosujemy regu�y dop�ki co� si� dzieje
				if(useRules && CConfig::rulesUseNum > 0)
				{
					int times = 0;
					bool anyChange;
					do
					{
						sentence->FirstPosition();
						anyChange = false;
						do
						{
							if((int)sentence->GetCurrentToken()->GetLexCount() > 1)
								anyChange = anyChange || rules->Apply(*sentence);
						}
						while(sentence->NextPosition());
						if(anyChange) times++;
					}
					while(anyChange);
					if(times > 1) multipleRuleUse++;
				}

				sentence->FirstPosition();

				if (!layers.GotoFirstLayer())
					throw tagexception("There are no attributes in current layer");
				//layers iteration
				do
				{
					CValue mask;
					layers.GetLayerMask(mask);

					//--------------------------------------------------------------------
					if(layer > 0)
						this->tagger->Disambiguate(*sentence,true,false,useRules);

					if (!sentence->FirstPosition())
						break;
					do
					{
						//Je�li wycieli�my wszystkie disamby (na skutek z�ej decyzji taggera)
						if(sentence->GetCurrentToken()->DisambCount() == CToken::none)
						{
							disambCut++;

							bool disambSaved = false;
							const CLexem& lDis = (*oryginal)[sentence->GetPosition()]->DisambLexem();
							CValue disVal,tempVal;
							lDis.GetMaskedValue(mask,disVal);
							CToken* tWrk = sentence->GetCurrentToken();

							for(int j = 0; j < (int)tWrk->GetLexCount(); j++)
							{
								tWrk->GetLexem(j)->GetMaskedValue(mask,tempVal);
								if(disVal == tempVal)
								{
									tWrk->GetLexem(j)->SetDisamb(true);
									disambCutSaved++;
									disambSaved = true;
									break;
								}
							}

							if(!disambSaved)
								continue;
						}

						if (!CFilter::IsAmb(*sentence,layers))
							continue;

						CValue p(0,0);

						p = (*sentence->GetCurrentToken())[mask];

						// ---------------------------------------------------------------------
						// ALGORYTM PRZYDZIA�U PRZYP. UCZ�CYCH DO KUBE�K�W.

						map<CValue,JOSKIPI::Patterns::CPattern*>::iterator pIter;

						int bitNumber = INT_MAX;

						for(pIter = patterns->begin(); pIter != patterns->end(); pIter ++)
						{
							if ( (p.first & pIter->first.first) == p.first 
								&& (p.second & pIter->first.second) == p.second) 
							{
								if (Parser::BitCount(pIter->first.first) 
									+ Parser::BitCount(pIter->first.second)	< bitNumber)
								{
									bitNumber = Parser::BitCount(pIter->first.first)
										+ Parser::BitCount(pIter->first.second);
								}
							}
						}

						bool isContained = false;

						for(pIter = patterns->begin(); pIter != patterns->end(); pIter ++)
						{
							if ( (p.first & pIter->first.first) == p.first 
								&& (p.second & pIter->first.second) == p.second) 
							{
								int bits = Parser::BitCount(pIter->first.first) 
									+ Parser::BitCount(pIter->first.second);
								if ( bits <= bitNumber + treeGenerationLevel)
								{
									ofstream* ss = (*patternsOut)[pIter->first];
									(*patterns)[pIter->first]->GeneratePattern(mask,*sentence,*ss,disambOnly);
									isContained = true;
								}
							}
						}

						if(!isContained)
						{
							//p.first = 0;
							//p.second = 0;
							//ofstream* ss = patternsOut[p];
							//patterns[p]->GeneratePattern(layers,*sentence,*ss);
							notContained ++;
						}

						// ---------------------------------------------------------------------
					}
					while(sentence->NextPosition());
				}
				while(layers.GotoNextLayer());

				if (sentence)
					delete sentence;

				i++;
				if(i%2000 == 0)
				{
					cout << endl;
					cout << "Wczytano " << i << " zdan.\n";
					cout << "Nie zawarto w drzewach " << notContained << " wyrazow.\n";
					cout << "Wycieto disamba " << disambCut << " razy, z czego uratowano " 
						<< disambCutSaved << " sztuk.\n";
					cout << "Wycieto lexemow regulami " << JOSKIPI::Rules::CAction::counter << endl;
				}
				delete oryginal;
			}

			if(useRules)
			{
				std::string ruleInfoFileName("RulesInfo.txt"); // TODO: extract to config!
				std::string ruleInfoPath = IO::GetFullPath(CConfig::commonPath,
					CConfig::ruleSubDir, ruleInfoFileName);
				rules->GetRulesInfo(ruleInfoPath.c_str());
				//std::cerr << "########\n####\n########### Multiple rule use: " << multipleRuleUse << "\n";
			}
			SaveFiles(normalTrees);
			switch(CConfig::classifierType)
			{
			case CConfig::LMT:
				SaveLMTFiles();
				break;
			case CConfig::Ripper:
				SaveJRipFiles();
				break;
			case CConfig::PART:
				SavePartFiles();
				break;
			}

			//system("Koder.exe");
		}

		void CTeacher::CreateFiles()
		{
			std::string fileName;

			for(std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>::iterator i = patterns->begin(); i != patterns->end(); i++)
			{
				std::stringstream fnamebuff;
				fnamebuff << IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir)
						<< IO::GetSeparator()
						<< i->first.first << "_" << i->first.second
						<< ".data";
				
				fileName = fnamebuff.str();

				std::ofstream *file = new std::ofstream();
				file->open(fileName.c_str());

				patternsOut->insert(std::pair<Corpus::CValue, std::ofstream*>(i->first,file));
				//file->close(); !!! THIS PRODUCED ERRORS!
				//delete file; !!!
			}
		}

		void CTeacher::SaveFiles(bool normalTrees)
		{
			using namespace std;
			IO::MakeSubdir(CConfig::treeSubDir); // DONE: from config
			stringstream names;
			stringstream namesStrings;
			string namesFile;
			string c45command;

			for(map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>::iterator i = patterns->begin(); i != patterns->end(); i++)
			{
				if((*i).second->GetPatternSize() == 0)
					continue;

				// DONE: tree path from config
				std::stringstream fnamebuff;
				
				//fileName = IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir);
				//fileName.append(IO::GetSeparator());
				fnamebuff << IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir)
						<< IO::GetSeparator()
						<< i->first.first << "_" << i->first.second;

				(i->second)->GenerateValuesPattern(names);
				
				std::string filename = fnamebuff.str();
				IO::WriteToFile((filename + ".names").c_str(),names);
				/*

				namesStrings << i->first.first << "\t" << i->first.second 
					<< "\t" << i->second->GetPatternSize() << "\t";

				for(Corpus::DLONG j = 1; j <= Corpus::last_cf; j *= 2)
				{
					if ( ((*i).first.first & j) != 0 )
					{
						namesStrings << Corpus::CTag::GetFlexClassString(j) << "\t";
					}
				}
				for(int j = 3; j < (int)Corpus::CToken::END; j ++)
				{
					Corpus::CTag::GetAttribNames(j,namesStrings,i->first);
				}
				namesStrings << endl;
				*/
			}
			//IO::WriteToFile("Classes.txt",namesStrings);
		}

		void CTeacher::InitFromFile(std::string fileName)
		{
			JOSKIPI::Parsers::CAntlrParser parser;

			std::ifstream file;
			file.open(IO::GetFullPath(CConfig::commonPath, fileName).c_str());
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

			fileName.append(".txt");
			std::string filePath = IO::GetFullPath(CConfig::commonPath, fileName);

			Corpus::TextUtils::CorpusUtil::CDump* patternDump =
				new Corpus::TextUtils::CorpusUtil::CDump(filePath.c_str());

			for(std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>::iterator iter = patterns->begin();
				iter != patterns->end(); iter++)
			{
				patternDump->Dump((char*)iter->second->ToString().c_str());
			}

			delete patternDump;
		}

		//AW!
		void CTeacher::SaveLMTFiles()
		{
			using namespace std;
			using namespace Corpus;
			IO::MakeSubdir(CConfig::treeSubDir); //DONE: path from config
			stringstream arff;
			stringstream namesStrings;
			string arffFile;
			string dataFileName;
			string lmtcommand;
			ifstream dataFile;


			for(map<CValue,JOSKIPI::Patterns::CPattern*>::iterator i = patterns->begin(); i != patterns->end(); i++)
			{
				if((*i).second->GetPatternSize() == 0)
					continue;		

				// DONE: tree path from config
				std::stringstream fnamebuff;
				fnamebuff << IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir)
						<< IO::GetSeparator()
						<< i->first.first << "_" << i->first.second;
				
				std::string filename = fnamebuff.str();
				(i->second)->GenerateArffValuesPattern(arff);
				dataFileName = filename + ".data";
				dataFile.open(dataFileName.c_str());
				arff << dataFile.rdbuf();
				dataFile.close();

				arffFile = filename + ".arff";
				IO::WriteToFile(arffFile.c_str(),arff);

				cout << "Zapisuje plik " << filename << ".LMT" << endl; 

				/*
				lmtcommand = "java -Xmx1448m -classpath \".\\weka-tuned.jar;\" weka.classifiers.trees.lmt.LMTLearner -t ";
				lmtcommand.append(arffFile);
				lmtcommand.append(" -M 50");
				//lmtcommand.append(" -C");
				lmtcommand.append(" > ");
				lmtcommand.append(fileName);
				lmtcommand.append(".LMT");

				*/

				//uzewnetrznienie nauki
				lmtcommand = "TeacherLMT.bat ";// TODO: extract to config!
				lmtcommand.append(filename);
				system(lmtcommand.c_str());
				//--
				system(lmtcommand.c_str());

				namesStrings << i->first.first << "\t" << i->first.second 
					<< "\t" << i->second->GetPatternSize() << "\t";

				// TODO
				for(DLONG j = 1; j <= Corpus::last_cf; j *= 2)
				{
					if ( ((*i).first.first & j) != 0 )
					{
						namesStrings << CTag::GetFlexClassString(j) << "\t";
					}
				}
				for(int j = 3; j < (int)Corpus::CToken::END; j ++)
				{
					CTag::GetAttribNames(j,namesStrings,i->first);
				}
				namesStrings << endl;
			}
			//IO::WriteToFile("Classes.txt",namesStrings);
		}

		//AW!
		void CTeacher::SavePartFiles()
		{
			using namespace std;
			using namespace Corpus;
			IO::MakeSubdir(CConfig::treeSubDir); //DONE: path from config
			stringstream arff;
			stringstream namesStrings;
			string fileName;
			string arffFile;
			string dataFileName;
			string lmtcommand;
			ifstream dataFile;


			for(map<CValue,JOSKIPI::Patterns::CPattern*>::iterator i = patterns->begin(); i != patterns->end(); i++)
			{
				if((*i).second->GetPatternSize() == 0)
					continue;

				// DONE: tree path from config
				std::stringstream fnamebuff;
				fnamebuff << IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir)
						<< IO::GetSeparator()
						<< i->first.first << "_" << i->first.second;

				std::string filename = fnamebuff.str();
				(i->second)->GenerateArffValuesPattern(arff);
				dataFileName = fileName + ".data";
				dataFile.open(dataFileName.c_str());
				arff << dataFile.rdbuf();
				dataFile.close();

				arffFile = filename + ".arff";
				IO::WriteToFile(arffFile.c_str(),arff);

				cout << "Zapisuje plik " << fileName << ".PART" << endl; 

				/*
				lmtcommand = "java -Xmx1000m -classpath \".\\weka-tuned.jar;\" weka.classifiers.rules.PART -t ";
				lmtcommand.append(arffFile);
				lmtcommand.append(" > ");
				lmtcommand.append(fileName);
				lmtcommand.append(".PART");
				system(lmtcommand.c_str());
				*/
				//uzewnetrznienie nauki
				lmtcommand = "TeacherPART.bat ";// TODO: extract to config!
				lmtcommand.append(fileName);
				system(lmtcommand.c_str());
				//--
				namesStrings << i->first.first << "\t" << i->first.second 
					<< "\t" << i->second->GetPatternSize() << "\t";

				for(DLONG j = 1; j <= Corpus::last_cf; j *= 2)
				{
					if ( ((*i).first.first & j) != 0 )
					{
						namesStrings << CTag::GetFlexClassString(j) << "\t";
					}
				}
				for(int j = 3; j < (int)Corpus::CToken::END; j ++)
				{
					CTag::GetAttribNames(j,namesStrings,i->first);
				}
				namesStrings << endl;
			}
			IO::WriteToFile("Classes.txt",namesStrings);// TODO: extract to config!
		}
		//AW!
		void CTeacher::SaveJRipFiles()
		{
			using namespace std;
			using namespace Corpus;
			IO::MakeSubdir(CConfig::treeSubDir); //DONE: path from config
			stringstream arff;
			stringstream namesStrings;
			string fileName;
			string arffFile;
			string dataFileName;
			string lmtcommand;
			ifstream dataFile;

			for(map<CValue,JOSKIPI::Patterns::CPattern*>::iterator i = patterns->begin(); i != patterns->end(); i++)
			{
				if((*i).second->GetPatternSize() == 0)
					continue;

				std::stringstream fnamebuff;
				fnamebuff << IO::GetFullPath(CConfig::commonPath, CConfig::treeSubDir)
						<< IO::GetSeparator()
						<< i->first.first << "_" << i->first.second;

				std::string filename = fnamebuff.str();
				
				(i->second)->GenerateArffValuesPattern(arff);
				dataFileName = filename + ".data";
				dataFile.open(dataFileName.c_str());
				arff << dataFile.rdbuf();
				dataFile.close();

				arffFile = filename + ".arff";
				IO::WriteToFile(arffFile.c_str(),arff);
				cout << "Zapisuje plik " << filename << ".RIP" << endl; 

				lmtcommand = "TeacherRIP.bat ";// TODO: extract to config!
				lmtcommand.append(filename);
				system(lmtcommand.c_str());
				//--

				namesStrings << i->first.first << "\t" << i->first.second 
					<< "\t" << i->second->GetPatternSize() << "\t";

				for(DLONG j = 1; j <= Corpus::last_cf; j *= 2)
				{
					if ( ((*i).first.first & j) != 0 )
					{
						namesStrings << CTag::GetFlexClassString(j) << "\t";
					}
				}
				for(int j = 3; j < (int)Corpus::CToken::END; j ++)
				{
					CTag::GetAttribNames(j,namesStrings,i->first);
				}
				namesStrings << endl;
			}
			IO::WriteToFile("Classes.txt",namesStrings);// TODO: extract to config!
		}

	}//namespace Learning 
}//	namespace TaKIPI
