#include "../joskipi_headers.h"
#include "ComplexCond.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CComplexCond::CComplexCond(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions)
			:CCondition(positions),conditions(conditions)
		{
		}

		CComplexCond::~CComplexCond(void)
		{
			if(conditions)
			{
				for(int i = 0; i < (int)conditions->size(); i++)
				{
					CCondition* c = conditions->at(i);
					if(c)
						delete c;
				}
				delete conditions;
			}
		}
	}
}
