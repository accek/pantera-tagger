#include "../joskipi_headers.h"
#include "IsBigSC.h"


namespace JOSKIPI
{
	namespace Operators
	{
		CIsBigSC::CIsBigSC(std::vector<CCondElem*>& cndElems) throw (std::string)
			:CSimpleCondition(cndElems)
		{
			if((int)cndElems.size() != 1)
				throw std::string("Wrong amount od condition elements.");
		}

		CIsBigSC::~CIsBigSC(void)
		{
		}

		bool CIsBigSC::Evaluate(Corpus::CSentence& sentence)
		{
			std::vector<Corpus::CValue> v;
			cndElems[0]->GetValue(v,sentence);
			std::string temp = Corpus::CWordDictionary::GetInstance()->GetAt((int)v[0].first);

			return Corpus::Parser::IsBig(temp);
		}

		std::string CIsBigSC::ToString()
		{
			std::stringstream ss;

			ss << "isbig(" << cndElems[0]->ToString() << ")";

			return ss.str();
		}
	}
}
