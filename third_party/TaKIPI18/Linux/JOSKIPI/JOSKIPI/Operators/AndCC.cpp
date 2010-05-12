#include "../joskipi_headers.h"
#include "AndCC.h"

namespace JOSKIPI
{

	namespace Operators
	{
		CAndCC::CAndCC(std::vector<CCondition*>* conditions)
			:CComplexCond(0,conditions)
		{
		}

		CAndCC::~CAndCC(void)
		{
		}

		bool CAndCC::Evaluate(Corpus::CSentence& sentence)
		{
			for(int i = 0; i < (int)conditions->size(); i++)
			{
				if (!conditions->at(i)->Evaluate(sentence))
					return false;
			}

			return true;
		}

		std::string CAndCC::ToString()
		{
			std::stringstream ss;

			ss << "and(" << std::endl;

			for(int i = 0; i < (int)conditions->size(); i++)
			{
				ss << (*conditions)[i]->ToString();
				if( i == conditions->size() -1 )
					ss << std::endl;
				else 
					ss << "," << std::endl;
			}
			ss << ") //and\n";

			return ss.str();
		}
	}

}
