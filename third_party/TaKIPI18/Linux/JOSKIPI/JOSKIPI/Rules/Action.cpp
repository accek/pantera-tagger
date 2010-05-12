#include "../joskipi_headers.h"
#include "Action.h"


namespace JOSKIPI
{
	namespace Rules
	{
		long CAction::counter = 0;

		CAction::CAction(Operators::CCondition* cnd)
		{
			if(cnd == 0)
				throw Corpus::tagexception("There must be a condition.");
			cond = cnd;
			lastFiredCount = 0;
		}

		CAction::~CAction(void)
		{
			delete cond;
		}
		
		int CAction::GetLastFiredCount()
		{
			return lastFiredCount;
		}

		bool CAction::Delete(Corpus::CSentence& sentence)
		{
			std::vector<Corpus::CLexem*> evaluatedLexems,tempLexems;

			// Wszystkie leksemy wrzucamy do wektora.
			for(int i = 0; i < (int)sentence.GetCurrentToken()->GetLexCount(); i++)
			{
				tempLexems.push_back(sentence.GetCurrentToken()->GetLexem(i));
				// Chwilowo, s� dwa miejsca gdzie przechowujemy lexemy, dlatego trzeba 
				// to odnotowa�.
				sentence.GetCurrentToken()->GetLexem(i)->RefCount++;
			}
			// Czyszcz�c leksemy zmniejszamy ich referencj� i z powrotem mamy 1
			sentence.GetCurrentToken()->ClearLexems();

			bool cutDisamb = false;
			lastFiredCount = 0;

			for(int i = 0; i < (int)tempLexems.size(); i++)
			{
				sentence.GetCurrentToken()->AddLexem(tempLexems[i]);
				// Ten lexem pojawia si� znowu w 2 miejscach, dlatego potrzebujemy
				// zwi�kszy� jego liczno�� wyst�pie� na 2.
				tempLexems[i]->RefCount++;

				if(!cond->Evaluate(sentence))
				{
					// warunek niespe�niony, wi�c pozostawiamy go w evaluatedLexems
					evaluatedLexems.push_back(tempLexems[i]);
					// Wyst�puje teraz w 3 miejscach.
					tempLexems[i]->RefCount++;
				}
				else
				{
					lastFiredCount++;
					if(tempLexems[i]->GetDisamb())
					{
						cutDisamb = true;
					}
				}

				//I z powrotem zmniejszamy jego liczb� wyst�pie�.
				sentence.GetCurrentToken()->ClearLexems();
			}

			if((int)evaluatedLexems.size() != 0)
			{
				for(int i = 0; i < (int)evaluatedLexems.size(); i++)
				{
					sentence.GetCurrentToken()->AddLexem(evaluatedLexems[i]);
				}
			}
			else
			{
				// wyci�li�my wszystko, przywracamy co by�o
				for(int i = 0; i < (int)tempLexems.size(); i++)
				{
					sentence.GetCurrentToken()->AddLexem(tempLexems[i]);
					tempLexems[i]->RefCount++;
				}
				lastFiredCount = 0;
				cutDisamb = false;
			}

			for(int i = 0; i < (int)tempLexems.size(); i++)
			{
				if(tempLexems[i]->RefCount == 1)
				{
					Corpus::CLexem* l = tempLexems[i];
					delete l;
				}
				else
					tempLexems[i]->RefCount--;
			}
			counter += lastFiredCount;
			return cutDisamb;
		}

		Operators::CCondition* CAction::GetCond()
		{
			return cond;
		}

		std::string CAction::ToString()
		{
			std::stringstream ss;

			ss << cond->ToString();

			return ss.str();
		}
	} // namespace Rules

} // namespace JOSKIPI
