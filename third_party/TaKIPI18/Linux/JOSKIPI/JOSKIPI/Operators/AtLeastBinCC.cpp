#include "../joskipi_headers.h"
#include "AtLeastBinCC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CAtLeastBinCC::CAtLeastBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions, int num)
			:CBinaryComplexC(positions,conditions)
		{
			this->num = num;
		}

		CAtLeastBinCC::~CAtLeastBinCC(void)
		{
		}

		bool CAtLeastBinCC::Evaluate(Corpus::CSentence& sentence)
		{
			using Corpus::CPosition;
			Corpus::CPosition start, end;

			CalculateLimits(sentence,start,end);

			int number = 0;

			for(int i = start.GetPos(); i <= end.GetPos(); i++)
			{
				varPos->SetPos(i);
				if(conditions->at(0)->Evaluate(sentence))
					number++;
			}

			return number >= num;
		}

		std::string CAtLeastBinCC::ToString()
		{
			using Corpus::CPosition;
			std::stringstream ss;

			ss << "atleast(" << startPos->ToString() << "," << endPos->ToString() << "," << varPos->ToString() 
				<< "," << (*conditions)[0]->ToString() << ", "<< num<<")";

			return ss.str();
		}

	}
}
