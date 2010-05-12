#include "../joskipi_headers.h"
#include "DisOper.h"


namespace JOSKIPI
{
	namespace Operators
	{
		CDisPosOper::CDisPosOper(pos_ptr position) throw (std::string)
			:COperator(position)
		{
			if(!position)
				throw std::string("position cannot be null.");
			values = new std::vector<Corpus::DLONG>();
		}

		CDisPosOper::CDisPosOper(const CDisPosOper& op)
		{
			this->position = op.position;
			values = new std::vector<Corpus::DLONG>();
			//*values = *(op.values);

		}

		const CDisPosOper& CDisPosOper::operator =(const CDisPosOper& op)
		{
			this->position = op.position;
			//*values = *(op.values);
			return *this;
		}

		CDisPosOper::~CDisPosOper(void)
		{
			if(values)
				delete values;
		}

		bool CDisPosOper::GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence)
		{
			Corpus::CToken* token = sentence[sentence.GetPosition()+position->GetPos()];

			Corpus::CValue p;
			val.clear();

			Corpus::CLexem* best = token->GetLexem(0);
			for(int i = 1; i < (int)token->GetLexCount(); i++)
			{
				if(token->GetLexem(i)->probability > best->probability)
					best = token->GetLexem(i);
			}

			p.first = best->GetTag().FlexClass;
			p.second = 0;

			if(COperator::collectNames)
			{
				std::vector<Corpus::DLONG>::iterator location;
				location = find(values->begin(),values->end(),p.first);
				if (location == values->end())
					values->push_back(p.first);
			}

			val.push_back(p);

			return true;
		}

		void CDisPosOper::GetValueNames(std::stringstream& out)
		{
			hadZeroDuringValuesGet = false;
			if(values->empty())
			{
				out << "0";
				hadZeroDuringValuesGet = true;
				return;
			}

			out << (*values)[0];
			if((*values)[0] == 0)
				hadZeroDuringValuesGet = true;

			for(int i = 1; i < (int)values->size(); i++)
			{
				if((*values)[i] == 0)
					hadZeroDuringValuesGet = true;
				out << "," << (*values)[i];
			}

			if(!hadZeroDuringValuesGet)
			{
				out << ",0";
				hadZeroDuringValuesGet = true;
			}
		}

		std::string CDisPosOper::ToString()
		{
			std::stringstream ss;

			ss << "dispos" << "[" << position->GetPos() << "]";

			return ss.str();
		}

		//------------------------------------------------------------------------------------

		CDisCatOper::CDisCatOper(pos_ptr position) throw (std::string)
			:COperator(position)
		{
			if(!position)
				throw std::string("position cannot be null.");
			values = new std::vector<Corpus::DLONG>();
		}

		CDisCatOper::CDisCatOper(const CDisCatOper& op)
		{
			this->position = op.position;
			values = new std::vector<Corpus::DLONG>();
			//*values = *(op.values);
		}

		const CDisCatOper& CDisCatOper::operator =(const CDisCatOper& op)
		{
			this->position = op.position;
			//*values = *(op.values);
			return *this;
		}

		CDisCatOper::~CDisCatOper(void)
		{
			if(values)
				delete values;
		}

		bool CDisCatOper::GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence)
		{
			Corpus::CToken* token = sentence[sentence.GetPosition()+position->GetPos()];

			Corpus::CValue p;
			val.clear();

			Corpus::CLexem* best = token->GetLexem(0);
			for(int i = 1; i < (int)token->GetLexCount(); i++)
			{
				if(token->GetLexem(i)->probability > best->probability)
					best = token->GetLexem(i);
			}

			p.second = best->GetTag().Categories;
			p.first = 0;

			if(COperator::collectNames)
			{
				std::vector<Corpus::DLONG>::iterator location;
				location = find(values->begin(),values->end(),p.first);
				if (location == values->end())
					values->push_back(p.first);
			}

			val.push_back(p);

			return true;
		}

		void CDisCatOper::GetValueNames(std::stringstream& out)
		{
			hadZeroDuringValuesGet = false;
			if(values->empty())
			{
				out << "0";
				hadZeroDuringValuesGet = true;
				return;
			}

			out << (*values)[0];
			if((*values)[0] == 0)
				hadZeroDuringValuesGet = true;

			for(int i = 1; i < (int)values->size(); i++)
			{
				if((*values)[i] == 0)
					hadZeroDuringValuesGet = true;
				out << "," << (*values)[i];
			}

			if(!hadZeroDuringValuesGet)
			{
				out << ",0";
				hadZeroDuringValuesGet = true;
			}
		}

		std::string CDisCatOper::ToString()
		{
			std::stringstream ss;

			ss << "discat" << "[" << position->GetPos() << "]";

			return  ss.str();
		}
	}
}
