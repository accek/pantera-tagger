#include "../../../corpuslib_headers.h"
#include "GuesserTest.h"
#include "../../Morfeusz/morfeusz.h"
#include "../../Config.h" // maska klas fleksyjnych
#include <set>

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace GuesserTest
		{
			// Miara precyzji.
			void PrecisionAndRecallTests(GuessingSystem::Guesser* guesser,
				std::map<std::string,int> &testList, bool takeNodeBefIgn, std::ostream *outfile,
				bool countTags, bool countBases)
			{
				int totalGLex = 0, totalTLex = 0, intersection = 0;
				int formsCounter = 0, ignoresCounter = 0;
				
				// Dla ka�dego s�owa...
				for(std::map<std::string,int>::iterator iter = testList.begin(); iter != testList.end(); iter++)
				{
					std::vector<Corpus::CToken *> morphed;
					Morfeusz::Analyse(morphed, iter->first.c_str());
					std::auto_ptr<Corpus::CToken> testToken(morphed[0]);
					// wzi�li�my pierwszy, wywalamy reszt� od razu (pewnie reszty nie ma, ale na wszelki wypadek)
					for(int tok_num = 1; tok_num < (int)morphed.size(); ++tok_num)
					{
						delete morphed[tok_num];
					}
					
					std::auto_ptr<Corpus::CToken> guessedToken(guesser->Guess(iter->first.c_str()));

					// czy bie��cy token nie zosta� rozpoznany (ign)
					// je�li tak, to nie zwi�kszamy od razu ignoresCounter
					// -- mo�e si� okaza�, �e klasa fleksyjna by�a i tak do odrzucenia
					bool ignNow = (guessedToken->GetLexCount() == 0
							|| guessedToken->GetLexem(0)->GetTag().FlexClass == Corpus::cf_ign);
					bool countNow = false;

					// tlex -- liczba leksem�w testowych; zawiera r�wnie� te do odfiltrowania
					int tlex = (int)testToken->GetLexCount();
					int glex = 0; // liczba leksem�w z wyj�cia odgadywacza; r�wnie� do przefiltrowania

					if(!ignNow) glex = (int)guessedToken->GetLexCount();

					if(countTags) // nie ma powt�rze�, wi�c nie musimy generowa� zbior�w
					{
						// zamiast "totalTLex += tlex" zwi�kszamy to stopniowo w p�tli (bo cz�� odrzucamy)
						for(int t_num = 0; t_num < tlex; t_num++)
						{
							if(testToken->GetLexem(t_num)->GetTag().FlexClass & Corpus::CConfig::flexClassesToGuess)
							{
								totalTLex++;
								countNow = true; // s� jakiekolwiek tagi, kt�re nas interesuj�
							}
						}
				
						// je�li po filtrowaniu testToken co� w og�le zosta�o (je�li nie, nie bierzemy tego pod uwag� i idziemy dalej)
						if(countNow)
						{
							for(int g_num = 0; g_num < glex; g_num++)
							{
								if(!(guessedToken->GetLexem(g_num)->GetTag().FlexClass & Corpus::CConfig::flexClassesToGuess))
									continue; // klasa fleksyjna nie spe�nia maski, nie bierzemy pod uwag�
								totalGLex++;
						
								for(int t_num = 0; t_num < tlex; t_num++)
								{
									if(!(testToken->GetLexem(t_num)->GetTag().FlexClass & Corpus::CConfig::flexClassesToGuess))
										continue; // klasa fleksyjna nie spe�nia maski, nie bierzemy pod uwag�
	
									if(GuesserTest::CompareLexems(guessedToken->GetLexem(g_num), testToken->GetLexem(t_num),
									   countTags, countBases))
									{
										intersection++;
										break;
									}
								}
							}
						}
					}
					else if(countBases) // generujemy zbiory form bazowych, po czym je por�wnujemy (unikamy powt�rze�)
					{
						std::set<std::string> guesBases, testBases, isectBases;

						for(int t_num = 0; t_num < tlex; t_num++)
						{ // filtrujemy leksemy testowe: odrzucamy te, kt�rych POS (flexClass) nie spe�nia maski
							if(testToken->GetLexem(t_num)->GetTag().FlexClass & Corpus::CConfig::flexClassesToGuess)
							{
								std::string base(testToken->GetLexem(t_num)->GetBase());
								Corpus::Parser::ToLowerCP1250(base);
								testBases.insert(base);
								countNow = true; // cho� jeden leksem spe�nia mask�
							}
						}
				
						if(countNow)
						{
							for(int g_num = 0; g_num < glex; g_num++)
							{
								if(guessedToken->GetLexem(g_num)->GetTag().FlexClass & Corpus::CConfig::flexClassesToGuess)
								{
									std::string base(guessedToken->GetLexem(g_num)->GetBase());
									Corpus::Parser::ToLowerCP1250(base);
									guesBases.insert(base);
								}
							}
	
							if(!ignNow)
							{
								std::insert_iterator<std::set<std::string> > inserter(isectBases, isectBases.begin());
								std::set_intersection(guesBases.begin(), guesBases.end(), testBases.begin(), testBases.end(), inserter);
								intersection += (int)isectBases.size();
								totalGLex += (int)guesBases.size();
							}
							
							totalTLex += (int)testBases.size();
						}
					} // if(countTags) else if(countBases)

					if(ignNow && countNow) ignoresCounter++;
					if(countNow) formsCounter++;
					if(formsCounter % 10000 == 0)
						std::cout << " - przerobiono " << formsCounter << " form\r";
				}
				std::cout << std::endl;
				
				std::cout << "Inters = " << intersection << std::endl;
				std::cout << "totalTLex = " << totalTLex << std::endl;
				std::cout << "totalGLex = " << totalGLex << std::endl; 
				double recall = (double)intersection / (double)totalTLex;
				double precision = (double)intersection / (double)totalGLex;

				double fMeasure = (2 * recall * precision) / (recall + precision);

				std::cout << "Form: " << formsCounter << ", ign: " << ignoresCounter << std::endl;
				std::cout << "Kompl: " << recall << ", dokl: " << precision << std::endl;
				std::cout << "F-measure:   " << fMeasure << std::endl;

				if(outfile != NULL)
				{
					*outfile << formsCounter << "\t" << ignoresCounter << "\t" << recall
						<< "\t" << precision << "\t" << fMeasure << std::endl;
				}
			}

			bool CompareLexems(CLexem *guesLex, CLexem *testLex, bool countTags, bool countBases)
			{
				return
					(!countTags ||
						(guesLex->GetTag().FlexClass == testLex->GetTag().FlexClass
						&& guesLex->GetTag().Categories == testLex->GetTag().Categories)
					)
					&& (!countBases ||
						Parser::EqualsIgnoreCaseCP1250(guesLex->GetBase(), testLex->GetBase())
					);
			}

			void TagProbabilityTest(GuessingSystem::Guesser* guesser, std::map<std::string,int> &testList,
				Readers::CCorporaReader &corpus, std::ostream *outfile)
			{
				CToken *corpusToken;
				int tokCount = 0;
				int disCount = 0;
				int lexCount = 0;
				int indCount = 0;

				// przegl�damy wszystkie tokeny z korpusu wzorcowego
				while((corpusToken = corpus.GetToken()) != NULL)
				{
					// interesuj� nas te, kt�rych orth wyst�puje w zbiorze testowym
					if(testList.find(corpusToken->GetOrth()) != testList.end())
					{
						tokCount++;
						if(tokCount % 1000 == 0) std::cout << tokCount << " form\r";

						std::auto_ptr<CToken> guessedToken(guesser->Guess(corpusToken->GetOrth().c_str(), true));

						if(guessedToken.get() == NULL || guessedToken->GetLexCount() == 0)
							continue;

						// wybieramy leksem o najwy�szym prawdopodobie�stwie (losujemy jeden z nich)
						std::vector< std::pair<CLexem *, double> > lexems;
						for(int lex = 0; lex < (int)guessedToken->GetLexCount(); lex++)
						{
							lexems.push_back(std::pair<CLexem *, double>
								(guessedToken->GetLexem(lex), guessedToken->GetLexProbability(lex)));
						}

						std::sort(lexems.begin(), lexems.end(), &CompareLexPairs);

						double maxprob = lexems[0].second;
						size_t cutIndex, limit = lexems.size();
						for(cutIndex = 1; cutIndex < limit && lexems[cutIndex].second >= maxprob; cutIndex++);

						// pr�ba: liczymy �redni� liczb� tag�w oraz tych o najwy�szym prawdopodobie�stwie
						lexCount += (int)limit;
						indCount += (int)cutIndex;

						int selected = 0;
#ifdef WIN32
						selected = rand() % ((int)cutIndex);
#endif
						CLexem *selLex = lexems[selected].first;
						// szukamy czy wybrany leksem wyst�puje w korpusie jako disamb
						for(int lex = 0; lex < (int)corpusToken->GetLexCount(); lex++)
						{
							CLexem *corpusLex = corpusToken->GetLexem(lex);
							if(corpusLex->GetDisamb() && CompareLexems(selLex, corpusLex, true, true))
							{
								disCount++;
								break;
							}
						}
					}
				}
				std::cout << "\nPrzeanalizowych tokenow: " << tokCount << "\n";
				std::cout << "Trafionych: " << disCount << "\n";
				std::cout << "Srednio odgadnietych leksemow: " << ((double)lexCount / (double)tokCount) << "\n";
				std::cout << "Srednio odgadnietych leksemow o najw. prawdop.: "\
					<< ((double)indCount / (double)tokCount) << "\n";
				std::cout << "Dokladnosc: " << ((double)disCount / (double)tokCount) << "\n";
			}

			bool CompareLexPairs(const std::pair<CLexem *, double> &p1, const std::pair<CLexem *, double> &p2)
			{
				return p1.second > p2.second;
			}
		}
	}
}
