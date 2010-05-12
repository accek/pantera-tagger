#include "../joskipi_headers.h"
#include "OrCC.h"

namespace JOSKIPI
{
	namespace Operators
	{

		COrCC::COrCC(std::vector<CCondition*>* conditions)
			:CComplexCond(0,conditions)
		{
		}

		COrCC::~COrCC(void)
		{
		}

		bool COrCC::Evaluate(Corpus::CSentence& sentence)
		{
			for(int i = 0; i < (int)conditions->size(); i++)
			{
				if (conditions->at(i)->Evaluate(sentence))
					return true;
			}

			return false;
		}

		std::string COrCC::ToString()
		{
			std::stringstream ss;

			ss << "or(" << std::endl;

			for(int i = 0; i < (int)conditions->size(); i++)
			{
				ss << (*conditions)[i]->ToString();
				if( i == conditions->size() -1 )
					ss << std::endl;
				else 
					ss << "," << std::endl;
			}
			ss << ")//or\n";

			return ss.str();;
		}
	}
}
