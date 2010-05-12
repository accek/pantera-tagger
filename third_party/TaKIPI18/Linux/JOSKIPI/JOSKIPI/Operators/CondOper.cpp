#include "../joskipi_headers.h"
#include "CondOper.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CCondOper::CCondOper(void)
		{
		}

		CCondOper::CCondOper(CCondition* cond, COperator* op) throw(std::string)
			:COperator(pos_ptr())
		{
			if(cond == 0)
				throw std::string("Condition cannot be null.");
			if(op == 0)
				throw std::string("Operator cannot be null.");

			this->test = cond;
			this->underCond = op;
		}

		CCondOper::~CCondOper(void)
		{
			delete test;
			delete underCond;
		}

		std::string CCondOper::ToString()
		{
			std::stringstream ss;

			ss << "?" << underCond->ToString() << "?" << test->ToString();

			return ss.str();
		}

		void CCondOper::GetValueNames(std::stringstream& out)
		{
			hadZeroDuringValuesGet = false;
			underCond->GetValueNames(out);
			if(!underCond->hadZeroDuringValuesGet)
			{
				out << "," << Corpus::tv_undefined;		
			}
			hadZeroDuringValuesGet = true;
		}

		bool CCondOper::GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence)
		{
			if (test->Evaluate(sentence))
				return underCond->GetValue(val,sentence);
			else
			{
				val.clear();
				val.push_back(Corpus::CValue(0,Corpus::tv_undefined));
				return false;
			}
		}

		CCondOper::CCondOper(const CCondOper &op)
		{
			this->position = op.position;

			this->test = op.test;
			this->underCond = op.underCond;
		}

		const CCondOper& CCondOper::operator =(const CCondOper &op)
		{
			this->position = op.position;

			this->test = op.test;
			this->underCond = op.underCond;

			return *this;
		}
	}
}
