#include "../joskipi_headers.h"
#include "OnlyBinCC.h"

namespace JOSKIPI
{
	namespace Operators
	{

		COnlyBinCC::COnlyBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions)
			:CBinaryComplexC(positions,conditions)
		{
		}

		COnlyBinCC::~COnlyBinCC(void)
		{
		}

		bool COnlyBinCC::Evaluate(Corpus::CSentence& sentence)
		{
			Corpus::CPosition start, end;
			CalculateLimits(sentence,start,end);

			for(int i = start.GetPos(); i <= end.GetPos(); i++)
			{
				varPos->SetPos(i);
				if(!conditions->at(0)->Evaluate(sentence))
					return false;
			}

			return true;
		}

		std::string COnlyBinCC::ToString()
		{
			std::stringstream ss;
			using Corpus::CPosition;

			ss << "only(";

			if(startPos->GetPos() == CPosition::beginning)
				ss << "begin";
			else if(startPos->GetPos() == CPosition::end)
				ss << "end";
			else 
				ss << startPos->GetPos();

			ss << ",";

			if(endPos->GetPos() == CPosition::beginning)
				ss << "begin";
			else if(endPos->GetPos() == CPosition::end)
				ss << "end";
			else 
				ss << endPos->GetPos();

			ss << ",";

			if(varPos->GetPos() == CPosition::nowhere)
				ss << "nowhere";

			ss << ",";

			ss << (*conditions)[0]->ToString() << ")";

			return ss.str();
		}
	}
}

