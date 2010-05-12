#include "../../../corpuslib_headers.h"

#include "TestingConsoleUI.h"
#include "../Guesser.h"
#include "../FormsIdentifier.h"
#include "../FrequencyList/FrequencyList.h"
#include "GuesserTest.h"
#include "../../Parser/Parser.h"
#include "../../Config.h"
#include "../../Morfeusz/morfeusz.h"

#include "../BaseEnding/BaseEnding.h"
#include "../BaseEnding/BaseEndingDict.h"

//TODO: remove it
#include "../../Readers/PlainTextReader.h"
#include "../../Sentencer.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace GuesserTest
		{
			TestingConsoleUI::TestingConsoleUI()
			{
				CConfig::inputEncoding = Parser::cp1250;
				Morfeusz::SetEncoding(MORFEUSZ_CP1250);
			}

			void TestingConsoleUI::MemTest()
			{
				std::cout << "sizeof void * = "
					<< sizeof(void *) << std::endl;
				std::cout << "sizeof char = "
					<< sizeof(char) << std::endl;
				std::cout << "sizeof UBYTE = "
					<< sizeof(UBYTE) << std::endl;
				std::cout << "sizeof INTEGER = "
					<< sizeof(INTEGER) << std::endl;
				std::cout << "sizeof UINTEGER = "
					<< sizeof(UINTEGER) << std::endl;
				std::cout << "sizeof DLONG = "
					<< sizeof(DLONG) << std::endl;
				std::cout << "sizeof EndingTree::EndingTreeNode = "
					<< sizeof(EndingTree::EndingTreeNode) << std::endl;
				std::cout << "sizeof EndingTagSet = "
					<< sizeof(EndingTagSet::EndingTagSet) << std::endl;
				std::cout << "sizeof TagCount = "
					<< sizeof(EndingTagSet::TagCount) << std::endl;
			}

			bool TestingConsoleUI::AskAndExecute()
			{
				std::string empty;
				std::cout << "Danych szukam w " << CConfig::GetFullPath(empty) << "\n";
				char line[10];
				std::cout << "[U]czyc, [z]gadywac, [t]estowac, wykonac [a]utomatyczne testy czy testy [p]robablistyczne? >";

				std::cin.getline(line, 10);
				switch(line[0])
				{
				case 'u':
				case 'U':
					return PrepareAndSerialize();
				case 't':
				case 'T':
					return DoTests();
				case 'a':
				case 'A':
					return DoTenfoldTests();
				case 'p':
				case 'P':
					return DoProbTests();
				case 'x':
				case 'X':
					return DoQuickTest();
				default:
					return QueryAndAnswer();
				}
			}

			/////////////////////////
			bool TestingConsoleUI::DoQuickTest()
			{
				std::string fileTrain("/home/eliasz/workspace/CorpusRep/corpus/Corpus/Data/tc01.txt");
				std::string fileTest("/home/eliasz/workspace/CorpusRep/corpus/Corpus/Data/t01.txt");
				
				Guesser *guesser = new Guesser(fileTrain.c_str());
				std::cout << "Obcinanie prefiksow\n";
				guesser->AttachLeafToIgn();
				
				//////////////////// TESTY
				std::map<std::string,int> testData;
				GuessingSystem::FrequencyList::LoadList(testData, fileTest, 10000);
				GuessingSystem::GuesserTest::PrecisionAndRecallTests(guesser, testData, true, NULL, true, true);

				delete guesser;
				return true;
			}
			
			bool TestingConsoleUI::DoTests()
			{
				char fileIn[200];
				bool fileError;
				bool countTags = true;
				bool countBases = true;

				std::cout << "Ktore czesci mowy uwzwgledniac?\n"
					<< "Standardowo: subst depr adj adja adjp adv fin praet impt imps inf pcon pant ger pact ppas\n"
					<< "> ";
				TFlexClass flexMask = ReadFlexMask(std::cin);
				if(flexMask != cf_none)
					CConfig::flexClassesToGuess = flexMask;

				std::cout << "Testy na [t]agach, formach [b]azowych czy [o]bu? >";
				std::cin.getline(fileIn, 200);
				if(fileIn[0] == 't' || fileIn[0] == 'T')
					countBases = false;
				if(fileIn[0] == 'b' || fileIn[0] == 'B')
					countTags = false;

				do
				{
					std::cout << "Sciezka do pliku z danymi testowymi >";
					std::cin.getline(fileIn, 200);
					if(fileIn[0] == 0)
					{
						std::cout << "Anulowano\n";
						return false;
					}
					std::ifstream ftest(fileIn);
					fileError = !ftest;
					if(fileError)
					{
						std::cout << "Nie mozna odczytac podanego pliku\n";
					}
					else
					{
						ftest.close();
					}
				}
				while(fileError);

				std::map<std::string,int> testData;
				GuessingSystem::FrequencyList::LoadList(testData, fileIn, 10000);

				std::cout << "Drzewo koncowek: " << CConfig::GetEndingTreePath() << std::endl
					<< "Tagsety " << CConfig::GetEndingTagSetPath() << std::endl;
				
				GuessingSystem::Guesser *guesser =
						new GuessingSystem::Guesser(CConfig::GetEndingTreePath().c_str(),
						CConfig::GetEndingTagSetPath().c_str());

				double thr = 0.0;
				std::cout << "Prog obcinania (0 = bez) >";
				std::cin >> thr;
				std::cout << "(" << thr << ")\n";
				if(thr > 0.0)
				{
					char prun2[2];
					std::cout << "Heurystyka 2? (ENTER = tak) >";
					std::cin.ignore(1000, '\n');
					std::cin.getline(prun2, 2);
					if(prun2[0] == 0)
					{
						std::cout << "Obcinanie (2) przy progu " << thr << std::endl;
						guesser->Pruning2(thr);
					}
					else
					{
						std::cout << "Obcinanie (1) przy progu " << thr << std::endl;
						guesser->Pruning(thr);
					}
				}

				std::cout << "Slownik koncowek form bazowych: "
						<< CConfig::GetBaseEndingDictPath << std::endl;
				
				std::ifstream fbedict(CConfig::GetBaseEndingDictPath().c_str(), std::ios::binary);
				if(!fbedict)
				{
					std::cout << "Nie mozna odczytac slownika koncowek form bazowych. Zamykamy...\n";
					delete guesser;
					return false;
				}
				else
				{
					fbedict.close();
					BaseEnding::BaseEndingDict::Deserialize(CConfig::GetBaseEndingDictPath().c_str());
				}

				// Testy
				GuessingSystem::GuesserTest::PrecisionAndRecallTests(guesser, testData, true, NULL, countTags, countBases);

				delete guesser;
				return true;
			}

			bool TestingConsoleUI::DoTenfoldTests()
			{
				bool countTags = true;
				bool countBases = true;
				bool byFlexSeparately = false;
				std::vector<TFlexClass> flexClassesToExamine;
				char answer[10];

				std::cout << "Osobne test dla kazdej czesci mowy (trwaja bardzo dlugo)?\n"
					"ENTER = nie, t = tak >";
				std::cin.getline(answer, 10);
				byFlexSeparately = (answer[0] == 't' || answer[0] == 'T');

				std::cout << "Ktore czesci mowy uwzwgledniac? ENTER = zestaw standardowy:\n"
					<< "subst depr adj adja adjp adv fin praet impt imps inf pcon pant ger pact ppas\n"
					<< "> ";
				if(byFlexSeparately)
				{
					// wektor bedzie zawierac wszystkie klasy kolejno
					FillVectorWithFlexClasses(std::cin, flexClassesToExamine);
				}
				else
				{
					TFlexClass flexMask = ReadFlexMask(std::cin);
					if(flexMask == cf_none)
						flexMask = CConfig::flexClassesToGuess;
					std::cout.setf(std::ios::hex, std::ios::basefield);
					std::cout << "(mask = " << flexMask << ")" << std::endl;
					std::cout.setf(std::ios::dec, std::ios::basefield);
					// wektor b�dzie zawiera� jedn� warto��: wszystkie klasy na raz
					flexClassesToExamine.push_back(flexMask);
				}

				std::cout << "Testy na [t]agach, formach [b]azowych czy [o]bu? >";
				std::cin.getline(answer, 10);
				if(answer[0] == 't' || answer[0] == 'T')
					countBases = false;
				if(answer[0] == 'b' || answer[0] == 'B')
					countTags = false;

				std::cout << "Testy automatyczne. Niestety sciezki wpisane sa na sztywno w kod...\n";
				char pathFileList[300] = ("D:\\data\\list.txt");
				std::string pathTestDump("D:\\data\\out.txt");

				std::vector<std::string> fileListTrain;
				std::vector<std::string> fileListTest;

				std::ifstream fileFileList(pathFileList);
				char buff[300];
				while(fileFileList.good())
				{
					fileFileList.getline(buff, 300);
					if(buff[0] != 0)
					{
						fileListTrain.push_back(std::string(buff));
						fileFileList.getline(buff, 300);
						fileListTest.push_back(std::string(buff));
					}
				}

				double thr = 0.0;
				bool heur2 = false;

				std::cout << "Prog obcinania (0 = bez) >";
				std::cin >> thr;
				std::cout << "(" << thr << ")\n";
				if(thr > 0.0)
				{
					char prun2[2];
					std::cout << "Heurystyka 2? (ENTER = tak) >";
					std::cin.ignore(1000, '\n');
					std::cin.getline(prun2, 2);
					if(prun2[0] == 0)
					{
						std::cout << "Obcinanie (2) przy progu " << thr << std::endl;
						heur2 = true;
					}
					else
					{
						std::cout << "Obcinanie (1) przy progu " << thr << std::endl;
						heur2 = false;
					}
				}

				std::ofstream outDump(pathTestDump.c_str());
				outDump << "Seria testow. ";
				if(countTags) outDump << "TAGS ";
				if(countBases) outDump << "BASES";
				outDump << std::endl << std::endl;

				for(size_t currentFlex = 0; currentFlex < flexClassesToExamine.size(); currentFlex++)
				{
					TFlexClass currentPOS = flexClassesToExamine[currentFlex];
					CConfig::flexClassesToGuess = currentPOS;
					if(byFlexSeparately)
					{
						
						std::cout << "### TESTY DLA KLASY " << CTag::GetFlexClassString(currentPOS)
							<< std::endl;
						outDump << "### TESTY DLA KLASY " << CTag::GetFlexClassString(currentPOS)
							<< std::endl;
					}

					outDump << "Nr testu\tNazwa\tForm testowych\tIgnow\tKompletnosc\tDokladnosc\tF-measure\n";
					for(unsigned int testNo = 0; testNo < fileListTrain.size(); testNo++)
					{
						std::cout << "### Test nr " << (testNo + 1) << std::endl;
						std::cout << fileListTrain[testNo] << " + " << fileListTest[testNo] << std::endl;

						CWordDictionary::GetInstance()->Clear();
						BaseEnding::BaseEndingDict::GetInstance()->Clear();

						std::map<std::string,int> testData;
						GuessingSystem::FrequencyList::LoadList(testData, fileListTest[testNo], 10000);
						std::cout << "testData: " << testData.size() << std::endl;

						Guesser *guesser = new Guesser(fileListTrain[testNo].c_str());
						guesser->AttachLeafToIgn();

						if(thr > 0.0)
						{
							if(heur2) guesser->Pruning2(thr);
							else guesser->Pruning(thr);
						}
						outDump << (testNo + 1) << "\t" << fileListTrain[testNo] << "\t";
						GuessingSystem::GuesserTest::PrecisionAndRecallTests(guesser, testData, true, &outDump, countTags, countBases);
						delete guesser;
					}
				}
				return true;
			}

			bool TestingConsoleUI::QueryAndAnswer()
			{
#ifdef WIN32
				GuessingSystem::FormsIdentifier *fi = new GuessingSystem::FormsIdentifier(
					Corpus::Parser::cp1250, Corpus::Parser::cp1250, GuessingSystem::FormsIdentifier::guessAll);
#else
				GuessingSystem::FormsIdentifier *fi = new GuessingSystem::FormsIdentifier(
					Corpus::Parser::utf8, Corpus::Parser::utf8, GuessingSystem::FormsIdentifier::guessAll);
#endif
				std::vector<Corpus::CToken *> tokens;

				while(true)
				{
					std::string s;
					std::cin >> s;
					if(s[0] == ':')
						break;
					fi->Identify(s, tokens);
					for(int i = 0; i < (int)tokens.size();i++)
					{
						std::cout << tokens[i]->ToXmlString(true) << std::endl;
					}
					FormsIdentifier::FreeTokenVector(tokens);

					std::cout << "--------------------------------------------------------------------------------\n";
				}
				delete fi;

				return true;
			}

			bool TestingConsoleUI::PrepareAndSerialize()
			{
				std::string fileIn, fileOut1, fileOut2, fileBEDictOut;
				char params[10];
				bool fileError, doAttach = true, doPrune = true;

				std::cout << "Uczenie odgadywacza morfo-syntaktycznego\n";
				BaseEnding::ClearStats();

				do
				{
					std::cout << "Sciezka do pliku uczacego >";
					std::getline(std::cin, fileIn);
					if(fileIn.empty())
					{
						std::cout << "Anulowano\n";
						return false;
					}
					std::ifstream ftest(fileIn.c_str());
					fileError = !ftest;
					if(fileError)
						std::cout << "Nie mozna odczytac podanego pliku\n";
					else
						ftest.close();
				}
				while(fileError);

				std::cout << "Wszystko z pruningiem (ENTER)? Tylko [d]rzewo? czy drzewo i [o]bcinanie prefiksow (zalecane)? >";
				std::cin.getline(params, 10);
				if(params[0] == 'o' || params[0] == 'O')
				{
					doPrune = false;
					std::cout << "Drzewo+prefiksy\n\n";
				}
				else if(params[0] == 'd' || params[0] == 'D')
				{
					doPrune = false;
					doAttach = false;
					std::cout << "Tylko drzewo\n\n";
				}

				Guesser *guesser = new Guesser(fileIn.c_str());
				if(doAttach)
				{
					std::cout << "Obcinanie prefiksow\n";
					guesser->AttachLeafToIgn();
				}
				if(doPrune)
				{
					double thr = 0.01;
					std::cout << "Ciecie przy progu >";
					std::cin >> thr;
					std::cout << "(" << thr << ")\n";

					guesser->Pruning2(thr);
				}


				std::cout << "Sciezka do zapisu drzewa koncowek >";
				std::getline(std::cin, fileOut1);
				if(fileOut1.empty())
				{
					fileOut1 = CConfig::GetEndingTreePath();
					std::cout << fileOut1 << std::endl;
				}
				std::cout << "Sciezka do zapisu tagsetow >";
				std::getline(std::cin, fileOut2);
				if(fileOut2.empty())
				{
					fileOut2 = CConfig::GetEndingTagSetPath();
					std::cout << fileOut2 << std::endl;
				}
				guesser->Serialize(fileOut1.c_str(), fileOut2.c_str());

				do
				{
					std::cout << "Sciezka do zapisu slownika koncowek form bazowych >";
					std::getline(std::cin, fileBEDictOut);
					if(fileBEDictOut.empty())
					{
						fileBEDictOut = CConfig::GetBaseEndingDictPath();
						std::cout << fileBEDictOut << std::endl;
					}
					
					fileError = !BaseEnding::BaseEndingDict::Serialize(fileBEDictOut.c_str());
				}
				while(fileError);

				std::ofstream testlog(CConfig::GetTestLogPath().c_str());
				guesser->DumpTreeStats(testlog);
				testlog.close();

				delete guesser;

				BaseEnding::DumpStats();
				return true;
			}

			bool TestingConsoleUI::DoProbTests()
			{
				bool fileError;
				std::string fileIn, fileCorpus;

				std::cout << "Test modelu probablistycznego na korpusie i danych testowych\n\n";
				do
				{
					std::cout << "Sciezka do pliku z danymi testowymi >";
					std::getline(std::cin, fileIn);
					if(fileIn.empty())
					{
						std::cout << "Anulowano\n";
						return false;
					}
					std::ifstream ftest(fileIn.c_str());
					fileError = !ftest;
					if(fileError) std::cout << "Nie mozna odczytac podanego pliku\n";
					else ftest.close();
				}
				while(fileError);

				std::ifstream fcorpus;
				do
				{
					std::cout << "Sciezka do korpusu wzorcowego >";
					std::getline(std::cin, fileCorpus);
					if(fileCorpus.empty())
						fileCorpus = "D:\\data\\FREK_TEST.xml";
					fcorpus.open(fileCorpus.c_str());
					fileError = !fcorpus;
					if(fileError) std::cout << "Nie mozna odczytac podanego pliku\n";
				}
				while(fileError);

				Readers::CCorporaReader corpus(fcorpus, false);

				std::map<std::string,int> testData;
				GuessingSystem::FrequencyList::LoadList(testData, fileIn, 10000);

				GuessingSystem::Guesser *guesser =
						new GuessingSystem::Guesser(CConfig::GetEndingTreePath().c_str(),
						CConfig::GetEndingTagSetPath().c_str());

				BaseEnding::BaseEndingDict::Deserialize();

				TagProbabilityTest(guesser, testData, corpus, NULL);

				delete guesser;

				return true;
			}
			
			TFlexClass TestingConsoleUI::ReadFlexMask(std::istream &in)
			{
				std::string buff;
				std::getline(std::cin, buff);

				std::string stringClass;
				TFlexClass outMask = 0;

				std::istringstream iss(buff, std::istringstream::in);

				while(iss.good())
				{
					iss >> stringClass;
					if(stringClass.length() != 0)
					{
						outMask |= CTag::GetFlexClass(stringClass);
					}
				}

				return outMask;
			}

			void TestingConsoleUI::FillVectorWithFlexClasses(std::istream &in, std::vector<TFlexClass> &vector)
			{
				std::string buff;
				std::getline(std::cin, buff);
				std::string stringClass;
				std::istringstream iss(buff, std::istringstream::in);
				while(iss.good())
				{
					iss >> stringClass;
					if(stringClass.length() != 0)
					{
						vector.push_back(CTag::GetFlexClass(stringClass));
					}
				}
			}

			void TestingConsoleUI::TempTest()
			{
				//Corpus::Parser::Encoding user_encoding = Corpus::Parser::utf8;
				Corpus::Parser::Encoding user_encoding = Corpus::Parser::cp1250;

				std::stringstream in;
				std::string string_text = "";
				if(user_encoding != Corpus::Parser::cp1250)
				string_text = Corpus::Parser::Convert(string_text, user_encoding, Corpus::Parser::cp1250);
				in << string_text;
				Readers::CPlainTextReader *reader = new Readers::CPlainTextReader(in, "");
				CSentencer sentr(*reader);
				CSentence *s;
				while((s = sentr.GetSentence()) != NULL)
				{
					std::cout << "SENT\n" << s->ToXmlString() << "\n\n";
					delete s;
				}
				delete reader;
			}
		}//namespace GuesserTest
	}
}
