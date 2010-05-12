#include "../joskipi_headers.h"
#include "InSC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CInSC::CInSC(std::vector<CCondElem*>& cndElems) throw(std::string)
			:CSimpleCondition(cndElems)
		{
			if ((int)cndElems.size() != 2)
				throw std::string("Wrong condition elements number.");
		}

		CInSC::~CInSC(void)
		{
		}

		bool CInSC::Evaluate(Corpus::CSentence& sentence)
		{
			using std::vector;
			using Corpus::CValue;

			vector<vector<CValue > > valBufTab;

			for(int i = 0; i < (int)cndElems.size(); i++)
			{
				vector<CValue> v;
				cndElems[i]->GetValue(v,sentence);
				valBufTab.push_back(v);
			}

			if ((int)valBufTab[0].size() == 1 && (int)valBufTab[1].size() == 1)
			{
				if ( valBufTab[0][0].second == str_msk && valBufTab[1][0].second == str_msk )
				{
					return	valBufTab[0][0].first == valBufTab[1][0].first;
				}
				if ( valBufTab[0][0].second == str_msk || valBufTab[1][0].second == str_msk )
				{
					return false;
				}
				else if ( ((valBufTab[0][0].first & valBufTab[1][0].first) == valBufTab[0][0].first
					&& valBufTab[0][0].first != 0)
					|| ((valBufTab[0][0].second & valBufTab[1][0].second) == valBufTab[0][0].second
					&& valBufTab[0][0].second != 0))
					return true;
				return false;
			}
			else
			{
				for(int i = 0; i < (int)valBufTab[0].size(); i++)
				{
					bool temp = false;
					for(int j = 0; j < (int)valBufTab[1].size(); j++)
					{
						if(valBufTab[0][i] == valBufTab[1][j])
						{
							temp = true;
							break;
						}
					}
					if(temp == false)
						return false;
				}
			}

			return true;
		}

		std::string CInSC::ToString()
		{
			std::stringstream ss;

			ss << "in(" << cndElems[0]->ToString() << "," 
				<< cndElems[1]->ToString() << ")";

			return ss.str();
		}
	}
}
