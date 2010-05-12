#include "../joskipi_headers.h"
#include "ConstValue.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CConstValue::CConstValue()
		{
			set = new std::vector<Corpus::CValue >();
		}

		CConstValue::~CConstValue(void)
		{
			delete set;
		}

		bool CConstValue::GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence &sentence)
		{
			val.clear();
			for(int i = 0; i < (int)set->size(); i++)
				val.push_back(Corpus::CValue((*set)[i].first,(*set)[i].second));

			return false;
		}

		std::string CConstValue::ToString()
		{
			std::stringstream ss;

			ss << "{" << (*set)[0].first << "/" << (*set)[0].second << "}";

			for(int i = 1; i <(int)set->size(); i++)
				ss << ",{" << (*set)[i].first << "/" << (*set)[i].second << "}";

			return ss.str();
		}

		void CConstValue::AddValue(Corpus::CValue p)
		{
			set->push_back(p);
		}
	}

}

