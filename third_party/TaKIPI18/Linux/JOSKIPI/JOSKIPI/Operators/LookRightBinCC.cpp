#include "../joskipi_headers.h"
#include "LookRightBinCC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CLookRightBinCC::CLookRightBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions)
			:CLookBinCC(positions,conditions)
		{
		}

		CLookRightBinCC::~CLookRightBinCC(void)
		{
		}

		bool CLookRightBinCC::Evaluate(Corpus::CSentence& sentence)
		{
			using Corpus::CPosition;
			Corpus::CPosition s, e;

			CalculateLimits(sentence,s,e);

			for(int i = s.GetPos(); i <= e.GetPos(); i++)
			{
				varPos->SetPos(i);

				if(conditions->at(0)->Evaluate(sentence))
					return true;
			}
			varPos->nowhere;
			return false;
		}

		std::string CLookRightBinCC::ToString()
		{
			std::stringstream ss;
			using Corpus::CPosition;

			ss << "rlook("<< startPos->ToString() << "," << endPos->ToString() << "," << varPos->ToString() << ",";

			ss << (*conditions)[0]->ToString() << ")";

			return ss.str();
		}
	}
}

