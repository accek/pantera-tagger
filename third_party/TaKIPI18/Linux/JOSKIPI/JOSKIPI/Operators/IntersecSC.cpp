#include "../joskipi_headers.h"
#include "IntersecSC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CIntersecSC::CIntersecSC(std::vector<CCondElem*>& cndElems)throw (std::string)
			:CSimpleCondition(cndElems)
		{
			if ((int)cndElems.size() != 2)
				throw std::string("Wrong condition elements number.");
		}

		CIntersecSC::~CIntersecSC(void)
		{
		}

		bool CIntersecSC::Evaluate(Corpus::CSentence& sentence)
		{
			using std::vector;
			using Corpus::CValue;

			vector<vector<CValue> > valBufTab;

			for(int i = 0; i < (int)cndElems.size(); i++)
			{
				vector<CValue> v;
				cndElems[i]->GetValue(v,sentence);
				valBufTab.push_back(v);
			}

			if ((int)valBufTab[0].size() == 1 && (int)valBufTab[1].size() == 1)
			{
				if ( valBufTab[0][0].second == str_msk )
					return valBufTab[0][0].first == valBufTab[1][0].first;
				if ( (valBufTab[0][0].first & valBufTab[1][0].first) != 0 ||
					(valBufTab[0][0].second & valBufTab[1][0].second) != 0 )
					return true;
				return false;
			}
			else
			{
				for(int i = 0; i < (int)valBufTab[0].size(); i++)
				{
					for(int j = 0; j < (int)valBufTab[1].size(); j++)
					{
						if(valBufTab[0][i] == valBufTab[1][j])
						{
							return true;
						}
					}
				}
			}

			return false;
		}	

		std::string CIntersecSC::ToString()
		{
			std::stringstream ss;

			ss << "inter(" << cndElems[0]->ToString() << "," 
				<< cndElems[1]->ToString() << ")";

			return ss.str();
		}
	}
}
