#include "../takipi_headers.h"
#include "Test.h"
#include "Tagger.h"
#include "Config.h"

namespace TaKIPI
{
	CTest::CTest(TaKIPI::CLayers& layers)
		:layers(layers)
	{
		layers.GetAllMasks(mask);
	}

	CTest::~CTest(void)
	{
	}

	bool CTest::IsAmb(Corpus::CSentence& sentence, int tokenNum)
	{
		Corpus::CValue tokenValue = (*sentence[tokenNum])[mask];
		Corpus::CValue lexemValue;

		sentence[tokenNum]->GetLexem(0)->GetMaskedValue(mask,lexemValue);
		if( lexemValue.first == tokenValue.first 
			&& lexemValue.second == tokenValue.second)
				return false;
		
		return true;
	}

	int CTest::testCollocations = 0;

	// Procedura testuj¹ca dane zdanie otagowane wzglêdem zdania z korpusu
	// Parmetry:
	//		- sentence			- zdanie wejœciowe, otagowane przez tager
	//		- orginalSentence	- zdanie oryginalne, z korpusu,
	// Return:
	//		- funkcja zwraca ile wyrazów w zdaniu jest Ÿle otagowanych
	int CTest::Test(Corpus::CSentence& sentence, Corpus::CSentence& orginalSentence)
	{
		int wynik = sentence.Size();

#ifdef WITH_COLLOCATIONS
		//added by P.K.
		if (TaKIPI::CConfig::testMode == TaKIPI::CConfig::OnlyCollocationTest)
		{
			CTest::testCollocations = CRTTTagger::indexOfCollocations.size();
		}
#endif

		// dla ka¿dego s³owa w zdaniu
		for(int i = 0; i < sentence.Size(); i++)
		{
			// sprawdzamy jaka niejednoznacznoœæ wystêpuje z s³owie nr i, wzglêdem
			// ustalonej maski, czyli tylko wzglêdem uczonych warstw
			Corpus::CValue ambClass = (*orginalSentence[i])[mask];

			// DO ZBIERANIA STATYSTYK POSZCZEGÓLNYCH KLAS NIEJEDNOZNACZNOŒCI
			if (statistics.find(ambClass) == statistics.end())
				statistics[ambClass] = std::pair<int,int>(0,0);
			statistics[ambClass].first++;

			// je¿eli wyraz nie jest niejednoznaczny, to przechodzimy do nastêpnego wyrazu
			if(!IsAmb(orginalSentence,i) || orginalSentence[i]->DisambCount() == Corpus::CToken::none)
			{

#ifdef WITH_COLLOCATIONS
				//added by P.K.
				if(TaKIPI::CConfig::testMode == TaKIPI::CConfig::OnlyCollocationTest)
				{
					std::set<int>::iterator iter = CRTTTagger::indexOfCollocations.find(i);
					if (iter == CRTTTagger::indexOfCollocations.end()) {}
					else CTest::testCollocations--;
				}
#endif
				

				wynik--;
				continue;
			}

			// szukamy lexemu w wyrazie o najwiêkszym prawdopodobieñstwie (ustalonym przez tager)
			Corpus::CLexem* l = sentence[i]->GetLexem(0);
			for(int k = 1; k < (int)sentence[i]->GetLexCount(); k++)
			{
				if (sentence[i]->GetLexem(k)->probability > l->probability)
					l = sentence[i]->GetLexem(k);
			}

			bool hasFound = false;
			Corpus::CValue lexValue;

			// sprawdzamy czy najlepszy lexem ze zdania otagowanego 
			// jest rozstrzygaj¹cy w zdaniu z korpusu
			for(int j = 0; j < (int)orginalSentence[i]->GetLexCount(); j++)
			{
				//hardcoded by kuki
				/*
					w unigramch dla nieznanych s³ow prawdobopdobieñstwo wszystkich lexemow 
					jest takie same takze potrzebujemy dodatkowej flagi ktora poinformuje nas
					ze najlepszy (czyli w tym przypadku pierwszy) lexem zosta³ juz znaleziony
				*/
				bool bFoundBest = false;
				for(int k = 0; k < (int)sentence[i]->GetLexCount(); k++)
				{
					
					if(l->probability == sentence[i]->GetLexem(k)->probability && bFoundBest == false)
					{
						l = sentence[i]->GetLexem(k);
						l->GetMaskedValue(mask,lexValue);
						bFoundBest = true;
					}
					else
					{
						continue;
					}
					
					if(orginalSentence[i]->GetLexem(j)->GetDisamb())
					{
						Corpus::CValue temp;
						(*orginalSentence[i]->GetLexem(j)).GetMaskedValue(mask,temp);
						if(lexValue.first == temp.first && lexValue.second == temp.second)
						{
#ifdef WITH_COLLOCATIONS
							//test kolokacji (added by P. K. )
							if(TaKIPI::CConfig::testMode == TaKIPI::CConfig::OnlyCollocationTest)
							{
								std::set<int>::iterator iter = CRTTTagger::indexOfCollocations.find(i);
								if (iter == CRTTTagger::indexOfCollocations.end()) {}
								else CTest::testCollocations--;
							}
#endif	

							wynik--;
							hasFound = true;
							break;
						}
					}
				}
				if(hasFound)
					break;
			}
			// DO ZBIERANIA STATYSTYK POSZCZEGÓLNYCH KLAS NIEJEDNOZNACZNOŒCI
			if(!hasFound)
			{
				statistics[ambClass].second++;
			}
		}

		return wynik;
	}

	int CTest::Test(Corpus::CSentence& sentence, std::vector<Corpus::CValue >& ambClasses,
		std::vector<Corpus::DLONG >& disAmbClasses, std::vector<int>& disAmbCount)
	{
		layers.GotoFirstLayer();
		layers.GotoFirst();

		int wynik = sentence.Size();

		for(int i = 0; i < sentence.Size(); i++)
		{
			Corpus::CLexem* l = sentence[i]->GetLexem(0);

			for(int k = 1; k < (int)sentence[i]->GetLexCount(); k++)
			{
				if (sentence[i]->GetLexem(k)->probability > l->probability)
					l = sentence[i]->GetLexem(k);
			}

			if (statistics.find(ambClasses[i]) == statistics.end())
				statistics[ambClasses[i]] = std::pair<int,int>(0,0);

			statistics[ambClasses[i]].first++;
			if ((*l)[Corpus::CToken::flex] == disAmbClasses[i] || disAmbCount[i] == Corpus::CToken::multi)
			{
				wynik--;
			}
			else
			{
				statistics[ambClasses[i]].second++;
			}
		}

		return wynik;
	}
	
	int CTest::TestInterTagged(Corpus::CSentence &testedSentence, Corpus::CSentence &referenceSentence)
	{
		int missed = 0;
		static int processed = 0;
		// dla ka¿dego s³owa w zdaniu
		for(int i = 0; i < testedSentence.Size(); i++)
		{
			Corpus::CToken *referenceToken = referenceSentence[i];
			Corpus::CToken *testedToken = testedSentence[i];
			// je¿eli wyraz nie jest w ogóle oznaczony w korpusie wzorcowym, uznajemy za trafione
			if(!ShallWeReject(referenceToken) && TestInterTagged(testedToken, referenceToken))
			{
				missed++;
			}
			processed += 1;
		}

		return missed;
	}
	
	bool CTest::TestInterTagged(Corpus::CToken *testedToken, Corpus::CToken *referenceToken)
	{
		Corpus::CValue ambClass;
		// Sprawdzanie (testowanie) odbywa siê tylko po tych warstwach, które zadaliœmy
		// sprawdzamy jaka niejednoznacznoœæ wystêpuje w s³owie nr i, wzglêdem
		// ustalonej maski, czyli tylko wzglêdem uczonych warstw
		ambClass = (*referenceToken)[mask];

		// DO ZBIERANIA STATYSTYK POSZCZEGÓLNYCH KLAS NIEJEDNOZNACZNOŒCI
		if (statistics.find(ambClass) == statistics.end())
			statistics[ambClass] = std::pair<int,int>(0,0);
		statistics[ambClass].first++;

		bool hasFound = false;
		
		for(int tl = 0; tl < (int)testedToken->GetLexCount(); tl++)
		{
			Corpus::CValue tlexValue, olexValue;
			Corpus::CLexem *tlex = testedToken->GetLexem(tl);
			if(!tlex->GetDisamb())
			{
				continue;
			}
			
			tlex->GetMaskedValue(mask, tlexValue);
			
			for(int ol = 0; ol < (int)referenceToken->GetLexCount(); ol++)
			{
				Corpus::CLexem *olex = referenceToken->GetLexem(ol);
				
				if(!olex->GetDisamb())
				{
					continue;
				}
				
				// rzutujemy wartoœci na maskê
				olex->GetMaskedValue(mask, olexValue);
				
				// jeœli równe, zmniejszamy punkty karne o ten token i pomijamy go
				if(tlexValue == olexValue)
				{
					hasFound = true;
					break;
				}
			}
			if(hasFound)
				break;
		}
		// DO ZBIERANIA STATYSTYK POSZCZEGÓLNYCH KLAS NIEJEDNOZNACZNOŒCI
		if(!hasFound)
		{
			statistics[ambClass].second++;
		}
		
		// zwracamy czy nietrafiony
		return !hasFound;
	}
	
	bool CTest::ShallWeReject(Corpus::CToken *referenceToken)
	{
		int num_lexems = referenceToken->GetLexCount();
		if(num_lexems == 0) // brakuje leksemów
			return true;
		bool anyDisamb = false;
		for(int lex_pos = 0; lex_pos < num_lexems; lex_pos++)
		{
			Corpus::CLexem *lex = referenceToken->GetLexem(lex_pos);
			if(lex->GetDisamb()) anyDisamb = true;
			if(lex->GetTag().FlexClass == Corpus::cf_ign) // ign
			{
				return true;
			}
		}
		if(!anyDisamb) // brakuje rozstrzygaj¹cych
			return true;
		
		// nie ma do czego siê przyczepiæ
		return false;
	}

	void CTest::PrintStatistics(std::ofstream& file)
	{
		for(std::map<Corpus::CValue,std::pair<int,int> >::iterator i = statistics.begin();
			i != statistics.end(); i++)
		{
			file << (*i).first.first << "\t" << (*i).first.second << "\t"
				<< (*i).second.first << "\t" << (*i).second.second << "\t";

			for(Corpus::DLONG j = 1; j <= 0x40000000; j *= 2)
			{
				if ( ((*i).first.first & j) != 0 )
				{
					file << Corpus::CTag::GetFlexClassString(j) << "\t";
				}
			}
		
			std::stringstream ss;
			for(int j = 3; j <= 15; j++)
			{
				Corpus::CTag::GetAttribNames(j,ss,Corpus::CValue((*i).first.first,(*i).first.second));
			}
			file << ss.str() << std::endl;

			file.flush();
		}
	}
}//TaKIPI

