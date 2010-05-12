#include "../joskipi_headers.h"
#include "CatFlt.h"


namespace JOSKIPI
{
	namespace Operators
	{
		CCatFlt::CCatFlt(pos_ptr position, CConstValue* constVal)
			:COperator(position),attribs(constVal)
		{
			values = new std::vector<Corpus::DLONG>();
		}

		CCatFlt::~CCatFlt(void)
		{	
			if(attribs)
				delete attribs;
			if(values)
				delete values;
		}

		bool CCatFlt::GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence)
		{
			using Corpus::CValue;
			using Corpus::CToken;
			using Corpus::CPosition;

			if(sentence.GetPosition() == CPosition::nowhere
				|| position->GetPos() == CPosition::nowhere)
			{
				val.clear();
				val.push_back(CValue(0,0));
				return false;
			}

			CToken* token = sentence[sentence.GetPosition()+position->GetPos()];

			CValue out(0,0);
			CValue p;
			val.clear();

			std::vector<Corpus::CValue > maskVec;
			attribs->GetValue(maskVec,sentence);

			for(int i = 0; i < (int)token->GetLexCount(); i++)
			{
				token->GetLexem(i)->GetMaskedValue(maskVec[0],p);

				if(p == CValue(0,0))
					continue;

				if( (maskVec[0].first == 0 || (maskVec[0].first != 0 && p.first != 0))
					&& (maskVec[0].second == 0  || (maskVec[0].second != 0 && p.second != 0)) ) 
				{
					token->GetLexem(i)->GetMaskedValue(maskVec[1],p);

					out.first  |= p.first;
					out.second |= p.second;
				}
			}

			if(COperator::collectNames)
			{
				std::vector<Corpus::DLONG>::iterator location;
				location = find(values->begin(),values->end(),out.second);
				if (location == values->end())
					values->push_back(out.second);
			}

			val.push_back(out);
			return false;
		}

		void CCatFlt::GetValueNames(std::stringstream& out)
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

		std::string CCatFlt::ToString()
		{
			std::stringstream ss;

			ss << "catflt(";

			ss << this->position->ToString();

			std::vector<Corpus::CToken*> t;
			Corpus::CSentence* s = new Corpus::CSentence(t);
			ss << "," << attribs->ToString() << ")";

			return ss.str();
		}
	}
}
