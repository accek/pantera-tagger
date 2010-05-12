#include "../joskipi_headers.h"
#include "HasNumSC.h"


namespace JOSKIPI
{
	namespace Operators
	{
		CHasNumSC::CHasNumSC(std::vector<CCondElem*>& cndElems)
			:CSimpleCondition(cndElems)
		{
		}

		CHasNumSC::~CHasNumSC(void)
		{
		}

		bool CHasNumSC::Evaluate(Corpus::CSentence& sentence)
		{
			std::vector<Corpus::CValue> v;
			cndElems[0]->GetValue(v,sentence);
			const std::string &temp = Corpus::CWordDictionary::GetInstance()->GetAt((int)v[0].first);

			for(int i = 0; i < (int)temp.length(); i++)
				if( temp[i] > 0 && isdigit(temp[i]))
					return true;

			return false;
		}

		std::string CHasNumSC::ToString()
		{
			std::stringstream ss;

			ss << "hasnum(" << cndElems[0]->ToString() << ")";

			return ss.str();
		}
	}
}
