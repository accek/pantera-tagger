#include "../joskipi_headers.h"
#include "LookBinCC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CLookBinCC::CLookBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions)
			:CBinaryComplexC(positions,conditions)
		{
			iniStartPos.SetPos(startPos->GetPos());
			iniEndPos.SetPos(endPos->GetPos());
		}

		CLookBinCC::~CLookBinCC(void)
		{
		}
	}
}
