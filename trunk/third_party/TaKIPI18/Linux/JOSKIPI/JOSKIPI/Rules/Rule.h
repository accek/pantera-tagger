#ifndef JOSKIPI_RULE_H
#define JOSKIPI_RULE_H

#include "../joskipi_headers.h"
#include "../all_operators.h"
#include "Action.h"

namespace JOSKIPI
{
	namespace Rules
	{
		class JOSKIPI_API CRule
		{
		public:
			CRule(Operators::CCondition* condition, CAction* action, int priority);
			// dumps data to file...
			CRule(Operators::CCondition* condition, CAction* action, int priority, std::string fileName);
			~CRule(void);

			/** Applies rule to sentence, which may delete some lexems.
			  * Pass offset of current sentence start (number of tokens that preceded the sentence)
			  * in order to get absolute token positions in rule dump files. */
			bool Apply(Corpus::CSentence& sentence, int debugOffset = -1);

			bool Test(Corpus::CSentence& sentence);

			std::string GetRuleInfo();

			std::string ToString();
			int GetDisambCutCount();
			int GetActionCount();

		protected:
			// Priorytet regu³y.
			int priority;
			// Akcja zapalana przez regu³ê.
			CAction* action;
			// Warunek wykonania akcji.
			Operators::CCondition* condition;
			// Dump
			Corpus::TextUtils::CorpusUtil::CDump* dump;
			int disambCutCount;
			int ruleActionCount;
		};
	} // namspace Rules
}// namespace JOSKIPI

#endif // JOSKIPI_RULE_H


