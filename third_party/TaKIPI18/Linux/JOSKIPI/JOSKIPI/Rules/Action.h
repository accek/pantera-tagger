#ifndef JOSKIPI_ACTION_H
#define JOSKIPI_ACTION_H

#include "../joskipi_headers.h"
#include "../all_operators.h"


namespace JOSKIPI
{
	namespace Rules
	{
		class JOSKIPI_API CAction
		{
		public:
			CAction(Operators::CCondition* cnd);
			~CAction(void);

			bool Delete(Corpus::CSentence& sentence);
			Operators::CCondition* GetCond();
			std::string ToString();
			static long counter;
			
			/// Retrieves the number the action was performed during last Delete call.
			int GetLastFiredCount();

		protected:
			Operators::CCondition* cond;
			int lastFiredCount;
		};
	} // namspace Rules
}// namespace JOSKIPI

#endif //JOSKIPI_ACTION_H


