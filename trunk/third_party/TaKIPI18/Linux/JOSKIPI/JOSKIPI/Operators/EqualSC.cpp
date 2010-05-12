#include "../joskipi_headers.h"
#include "EqualSC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CEqualSC::CEqualSC(std::vector<CCondElem*>& cndElems) throw (std::string)
			:CSimpleCondition(cndElems)
		{
			if ((int)cndElems.size() == 0)
				throw std::string("Wrong condition elements number.");
		}

		CEqualSC::~CEqualSC(void)
		{
		}

		bool CEqualSC::Evaluate(Corpus::CSentence& sentence)
		{
			// vector of value sets
			std::vector<std::set<Corpus::CValue > > valBufTab;

			for(int i = 0; i < (int)cndElems.size(); i++)
			{
				std::vector<Corpus::CValue> v;
				cndElems[i]->GetValue(v,sentence);
				std::set<Corpus::CValue> vset(v.begin(), v.end());
				valBufTab.push_back(vset);
			}

			std::set<Corpus::CValue> temp = valBufTab[0];
			for(int j = 1; j < (int)valBufTab.size(); j++)
			{
				if(temp != valBufTab[j])
					return false;
			}

			return true;
		}	

		std::string CEqualSC::ToString()
		{
			std::stringstream ss;

			ss << "equal(" << cndElems[0]->ToString() << "," 
				<< cndElems[1]->ToString() << ")";

			return ss.str();
		}
	}
}
