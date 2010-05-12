#include "../joskipi_headers.h"
#include "Condition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CCondition::CCondition(std::vector<pos_ptr>* positions)
		{
			this->posVec = positions;
		}

		CCondition::~CCondition()
		{
			delete posVec;
		}
	}

}

