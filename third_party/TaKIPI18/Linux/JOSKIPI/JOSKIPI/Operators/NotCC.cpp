#include "../joskipi_headers.h"
#include "NotCC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CNotCC::CNotCC(std::vector<CCondition*>* conditions)
			:CComplexCond(0,conditions)
		{
		}

		CNotCC::~CNotCC(void)
		{
		}

		bool CNotCC::Evaluate(Corpus::CSentence& sentence)
		{
			for(int i = 0; i < (int)conditions->size(); i++)
			{
				if (conditions->at(i)->Evaluate(sentence))
					return false;
			}

			return true;
		}

		std::string CNotCC::ToString()
		{
			std::stringstream ss;

			ss << "not(" << std::endl;

			for(int i = 0; i < (int)conditions->size(); i++)
			{
				ss << (*conditions)[i]->ToString();
				if( i == conditions->size() -1 )
					ss << std::endl;
				else 
					ss << "," << std::endl;
			}
			ss << ") //not \n";

			return ss.str();
		}

	}
}
