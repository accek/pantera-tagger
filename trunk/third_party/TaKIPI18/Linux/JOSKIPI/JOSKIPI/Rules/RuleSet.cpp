#include "../joskipi_headers.h"
#include "RuleSet.h"


namespace JOSKIPI
{
	namespace Rules
	{
		CRuleSet::CRuleSet(std::vector<CRule*>* rules)
			:rules(rules)
		{

		}

		CRuleSet::~CRuleSet(void)
		{
			for(int i = 0; i < rules->size(); ++i)
			{
				delete rules->at(i);
			}
			delete rules;
			rules = NULL;
		}


		void CRuleSet::AddRule(CRule* rule)
		{
			rules->push_back(rule);
		}

		bool CRuleSet::Apply(Corpus::CSentence& sentence, int debugOffset)
		{
			bool oneRuleApplied = false;

			for(int i = 0; i < (int)(*rules).size(); i++)
			{
				if((*rules)[i]->Apply(sentence, debugOffset))
					oneRuleApplied = true;
			}

			return oneRuleApplied;
		}

		bool CRuleSet::Test(Corpus::CSentence& sentence)
		{
			for(int i = 0; i < (int)(*rules).size(); i++)
			{
				if((*rules)[i]->Test(sentence))
					return true;
			}
			return false;
		}

		void CRuleSet::GetRulesInfo(std::string fileName)
		{
			std::ofstream file;
			file.open(fileName.c_str());

			int totalDisambCut = 0;
			int totalActionCount = 0;
			for(int i = 0; i < (int)(*rules).size(); i++)
			{
				file << (*rules)[i]->GetRuleInfo() << std::endl;
				totalDisambCut += (*rules)[i]->GetDisambCutCount();
				totalActionCount += (*rules)[i]->GetActionCount();
			}
			
			file << std::endl << "Lacznie bledow: " << totalDisambCut << " / " << totalActionCount
					<< " (" << ((double)totalDisambCut / totalActionCount) << ")" << std::endl;

			file.close();
		}

		std::string CRuleSet::ToString()
		{
			std::stringstream ss;

			for(int i = 0; i < rules->size(); ++i)
			{
				ss << "\ndelete(\n" << (*rules)[i]->ToString() << ")\n";
			}

			return ss.str();
		}
	}
}
