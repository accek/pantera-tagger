#include "../takipi_headers.h"
#include "CutOff.h"

namespace TaKIPI
{


	/*
	Zmienna statyczna do zliczania statystyk odcinania.
	*/
	long CCutOff::counter = 0;

	/*
	Konstrutkor dostaj¹cy poziom odciêcia jako parametr.
	*/
	CCutOff::CCutOff(double level)
	{
		cutOffLevel = log(level);
	}

	CCutOff::~CCutOff(void)
	{
	}

	//---------------------------------------------------------------------
	// Wycina ze zdania lexemy o prawdopodobieñstwie mniejszym od zadanego.
	//---------------------------------------------------------------------
	void CCutOff::CutOff(Corpus::CSentence& sentence)
	{
		// Dla ka¿dego tokenu.
		for(int j = 0; j < sentence.Size(); j++)
		{
			Corpus::CToken* t = sentence[j];

			// Dla ka¿dego lexemu (iteracja od koñca bo wyrzucamy.
			for(int i = (int)t->GetLexCount()-1; i >= 0; i--)
			{
				// Jeœli poni¿ej poziomu odciêcia
				if (t->GetLexem(i)->probability < cutOffLevel)
				{
					if ((int)t->GetLexCount() > 1)
					{
						t->RemoveLexem(i);
						counter++;
					}
					else
						return;
				}
			}
		}
	}

	void CCutOff::CutWorstPackage(Corpus::CSentence& sentence, TaKIPI::CLayers& layers)
	{
		for(int i = 0; i < sentence.Size(); i++)
		{
			Corpus::CValue mask,lexValue,worstLexVal;
			layers.GetLayerMask(mask);

			std::vector<Corpus::CLexem*> betterLexs;
			Corpus::CLexem* l = sentence[i]->GetLexem(0);

			for(int k = 1; k < (int)sentence[i]->GetLexCount(); k++)
			{
				if (sentence[i]->GetLexem(k)->probability < l->probability)
					l = sentence[i]->GetLexem(k);
			}

			l->GetMaskedValue(mask,worstLexVal);
			if((*sentence[i])[mask] == worstLexVal)
				continue;

			for(int k = 0; k < (int)sentence[i]->GetLexCount(); k++)
			{
				sentence[i]->GetLexem(k)->GetMaskedValue(mask,lexValue);
				if(worstLexVal != lexValue)
					betterLexs.push_back(sentence[i]->GetLexem(k));
				else
					counter++;
			}

			sentence[i]->ClearLexems();

			for(int k = 0; k < (int)betterLexs.size(); k++)
			{
				sentence[i]->AddLexem(betterLexs[k]);
			}
		}
	}

	void CCutOff::LeaveBestPackage(Corpus::CSentence& sentence, TaKIPI::CLayers& layers)
	{
		for(int i = 0; i < sentence.Size(); i++)
		{
			Corpus::CLexem* l = sentence[i]->GetLexem(0);

			for(int k = 1; k < (int)sentence[i]->GetLexCount(); k++)
			{
				if (sentence[i]->GetLexem(k)->probability > l->probability)
					l = sentence[i]->GetLexem(k);
			}

			std::vector<Corpus::CLexem*> bestPackLexs;

			Corpus::CValue mask,lexValue,worstLexVal;
			layers.GetLayerMask(mask);
			l->GetMaskedValue(mask,worstLexVal);

			for(int k = 0; k < (int)sentence[i]->GetLexCount(); k++)
			{
				sentence[i]->GetLexem(k)->GetMaskedValue(mask,lexValue);
				if(worstLexVal.first == lexValue.first &&
					worstLexVal.second == lexValue.second 
					|| sentence[i]->GetLexem(k)->probability == l->probability)
				{
					bestPackLexs.push_back(sentence[i]->GetLexem(k));
					sentence[i]->GetLexem(k)->RefCount++;
				}
			}

			sentence[i]->ClearLexems();

			for(int k = 0; k < (int)bestPackLexs.size(); k++)
			{
				sentence[i]->AddLexem(bestPackLexs[k]);
			}
		}
	}

}//namespace TaKIPI
