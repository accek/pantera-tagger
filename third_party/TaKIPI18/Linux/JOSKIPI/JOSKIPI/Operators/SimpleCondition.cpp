#include "../joskipi_headers.h"
#include "SimpleCondition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CSimpleCondition::CSimpleCondition(std::vector<CCondElem*>& cndElems)
			:CCondition(0),cndElems(cndElems)
		{
		}

		CSimpleCondition::~CSimpleCondition(void)
		{
			for(int i = 0; i < (int)cndElems.size(); i++)
			{
				if(cndElems[i])
					delete cndElems[i];
			}
		}
	}
}
