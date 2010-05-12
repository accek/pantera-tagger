#include "../joskipi_headers.h"
#include "TestOper.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CTestOper::CTestOper(void)
		{
		}

		CTestOper::CTestOper(CCondition* cond)
			:COperator(pos_ptr())
		{
			this->test = cond;
		}

		CTestOper::~CTestOper(void)
		{
			if(test)
				delete test;
		}

		std::string CTestOper::ToString()
		{
			return this->test->ToString();
		}

		void CTestOper::GetValueNames(std::stringstream& out)
		{
			hadZeroDuringValuesGet = false;
			out << Corpus::tv_false << "," << Corpus::tv_true;
		}

		bool CTestOper::GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence)
		{
			val.clear();
			val.push_back(Corpus::CValue(0,0));
			if (test->Evaluate(sentence))
			{
				val[0].second = Corpus::tv_true;
			}
			else
			{
				val[0].second = Corpus::tv_false;
			}

			return false;
		}

	}
}
