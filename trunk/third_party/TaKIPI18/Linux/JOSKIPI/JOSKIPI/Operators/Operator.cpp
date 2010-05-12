#include "../joskipi_headers.h"
#include "Operator.h"

namespace JOSKIPI
{
	namespace Operators
	{
		bool COperator::collectNames = true;

		COperator::COperator()
			:position(new Corpus::CPosition(Corpus::CPosition::nowhere))
		{
			hadZeroDuringValuesGet = false;
		}

		COperator::COperator(pos_ptr position)
			:position(position)
		{
			hadZeroDuringValuesGet = false;
		}

		COperator::~COperator(void)
		{
		}

		COperator::COperator(const COperator& op)
		{
			this->position = op.position;
			hadZeroDuringValuesGet = false;
		}

		const COperator& COperator::operator =(const COperator& op)
		{
			this->position = op.position;

			return *this;
		}
	}
}

