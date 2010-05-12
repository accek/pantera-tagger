#include "../joskipi_headers.h"
#include "OffsetPosition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		COffsetPosition::COffsetPosition(int offset, pos_ptr position) throw (std::string)
			:positionPtr(position)
		{
			if(!position)
				throw std::string("Position cannot be null.");
			this->offset = offset;
		}

		COffsetPosition::~COffsetPosition(void)
		{
		}

		int COffsetPosition::GetPos() const
		{
			if(offset > 0)
			{
				if(positionPtr->GetPos() == CPosition::end)
					return CPosition::end;
				else
					return positionPtr->GetPos() + offset;
			}
			else
			{
				if(positionPtr->GetPos() == CPosition::beginning)
					return CPosition::beginning;
				else
					return positionPtr->GetPos() + offset;
			}
		}
	}
}

