#include "../joskipi_headers.h"
#include "LookLeftBinCC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CLookLeftBinCC::CLookLeftBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions)
			:CLookBinCC(positions,conditions)
		{
		}

		CLookLeftBinCC::~CLookLeftBinCC(void)
		{
		}

		bool CLookLeftBinCC::Evaluate(Corpus::CSentence& sentence)
		{
			using Corpus::CPosition;
			Corpus::CPosition s, e;

			CalculateLimits(sentence,s,e);

			for(int i = s.GetPos(); i >= e.GetPos(); i--)
			{
				varPos->SetPos(i);

				if(conditions->at(0)->Evaluate(sentence))
					return true;
			}
			varPos->SetPos(CPosition::nowhere);
			return false;
		}

		std::string CLookLeftBinCC::ToString()
		{
			using Corpus::CPosition;
			std::stringstream ss;

			ss << "llook(" << startPos->ToString() << "," << endPos->ToString() << "," << varPos->ToString() << ",";

			ss << (*conditions)[0]->ToString() << ")";

			return ss.str();
		}
	}
}
