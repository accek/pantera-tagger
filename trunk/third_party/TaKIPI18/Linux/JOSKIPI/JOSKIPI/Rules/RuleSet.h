#ifndef JOSKIPI_RULESET_H
#define JOSKIPI_RULESET_H

#include "../joskipi_headers.h"
#include "Rule.h"

namespace JOSKIPI
{
	namespace Rules
	{
		class JOSKIPI_API CRuleSet
		{
		public:
			CRuleSet(std::vector<CRule*>* rules);

			~CRuleSet(void);

			void AddRule(CRule* rule);

			/** Tries to fire each rule against given sentence.
			  * Pass the number of tokens having preceded the sentence
			  * in order to get absolute token positions in rule dump files. */
			bool Apply(Corpus::CSentence& sentence, int debugOffset = -1);

			bool Test(Corpus::CSentence& sentence);

			void GetRulesInfo(std::string fileName);

			std::string ToString();

		protected:
			std::vector<CRule*>* rules;
		};
	}// namespace Rules

}// namespace JOSKIPI
#endif//JOSKIPI_RULESET_H

