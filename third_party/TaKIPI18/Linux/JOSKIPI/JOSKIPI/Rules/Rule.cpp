#include "../joskipi_headers.h"
#include "Rule.h"


namespace JOSKIPI
{
	namespace Rules
	{
		CRule::CRule(Operators::CCondition* condition, CAction* action, int priority)//, std::string fileName)
			:condition(condition), action(action), priority(priority)
		{
			disambCutCount = 0;
			ruleActionCount = 0;
			dump = NULL;
		}

		CRule::CRule(Operators::CCondition* condition, CAction* action, int priority, std::string fileName)
			:condition(condition), action(action), priority(priority)
		{
			dump = new Corpus::TextUtils::CorpusUtil::CDump(fileName.c_str());
			std::string s = action->GetCond()->ToString();
			s.append(" := ");
			s.append(condition->ToString());
			dump->Dump(s.c_str());
			disambCutCount = 0;
			ruleActionCount = 0;
		}

		CRule::~CRule(void)
		{
			delete action;
			delete condition;
			if(dump)
			{
				char buffer [256];
				sprintf (buffer, "Regula zadzialala %d razy wycinajac %d dissambow",ruleActionCount, disambCutCount);
				dump->Dump(buffer);
				delete dump;
			}
		}

		bool CRule::Test(Corpus::CSentence& sentence)
		{
			return condition->Evaluate(sentence);
		}

		std::string CRule::GetRuleInfo()
		{
			char buffer[256];
			sprintf(buffer,"Regula %d:\t%d / %d (%f).",
					priority, disambCutCount, ruleActionCount, ((float)disambCutCount / (float)ruleActionCount));
			return std::string(buffer);
		}
		
		int CRule::GetDisambCutCount()
		{
			return disambCutCount;
		}
		
		int CRule::GetActionCount()
		{
			return ruleActionCount;
		}

		std::string CRule::ToString()
		{
			std::stringstream ss;

			ss << action->ToString() << "\n# " << priority << " :-\n" << condition->ToString() << std::endl;

			return ss.str();
		}

		bool CRule::Apply(Corpus::CSentence& sentence, int debugOffset)
		{
			if(Test(sentence))
			{
				bool disambCut = action->Delete(sentence);
				bool ruleFired = (action->GetLastFiredCount() > 0);
				
				if(disambCut)
				{
					disambCutCount++;
					if(dump)
					{
						std::stringstream ss;
						ss << disambCutCount << "#" << ", wyraz " << sentence.GetPosition()
								<< " (" << sentence.GetCurrentToken()->GetOrth() << ")";
						
						if(debugOffset >= 0)
						{
							ss << " @" << (debugOffset + sentence.GetPosition());
						}
						dump->Dump(ss.str().c_str());
						dump->Dump(sentence);
					}
				}
				if(ruleFired) ruleActionCount++;
				return ruleFired;
			}
			return false;
		}

	} // namespace Rules

} // namespace JOSKIPI
