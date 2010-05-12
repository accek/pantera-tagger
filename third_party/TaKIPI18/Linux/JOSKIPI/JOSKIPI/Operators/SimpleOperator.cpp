#include "../joskipi_headers.h"
#include "SimpleOperator.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CSimpleOperator::CSimpleOperator()
		{
		}

		CSimpleOperator::CSimpleOperator(pos_ptr position, int attributeIndex) throw(std::string)
			:COperator(position)
		{
			if(!position)
				throw std::string("position cannot be null.");

			values = new std::vector<Corpus::DLONG>();

			attIndex = attributeIndex;
		}

		CSimpleOperator::CSimpleOperator(const CSimpleOperator& op)
		{
			this->attIndex = op.attIndex;
			this->position = op.position;
			values = new std::vector<Corpus::DLONG>();
			//*(this->values) = *(op.values); 
		}

		CSimpleOperator::~CSimpleOperator(void)
		{
			if(values)
			{
				delete values;
				values = NULL;
			}
		}

		const CSimpleOperator& CSimpleOperator::operator =(const CSimpleOperator& op)
		{
			this->attIndex = op.attIndex;
			this->position = op.position;
			//*(this->values) = *(op.values);
			return *this;
		}

		bool CSimpleOperator::GetValue(std::vector<Corpus::CValue>& val, Corpus::CSentence& sentence)
		{
			Corpus::CToken* token = sentence[sentence.GetPosition()+position->GetPos()];

			Corpus::CValue p;
			val.clear();
			bool score = true;

			if (Corpus::CToken::IsStrAttribute(attIndex))
			{
				if (attIndex == Corpus::CToken::orth)
				{
					if(Corpus::CWordDictionary::GetInstance()->Contains((unsigned int)(*token)[Corpus::CToken::orth]))
					{
						std::string s = token->GetOrth();
						p.first = Corpus::CWordDictionary::GetInstance()->Add(s);
						p.second = str_msk;
	
						if(COperator::collectNames)
						{
							std::vector<Corpus::DLONG>::iterator location;
							location = find(values->begin(),values->end(),p.first);
							if (location == values->end())
								values->push_back(p.first);
						}
						
						val.push_back(p);
					}
					else
					{
						val.push_back(Corpus::CValue(0,0));
					}
				}
				else //if base
				{
					for(int i = 0; i < (int)token->GetLexCount(); i++)
					{
						Corpus::CLexem *lex = token->GetLexem(i);
						if(Corpus::CWordDictionary::GetInstance()->Contains((unsigned int)(*lex)[Corpus::CToken::base]))
						{
							std::string s = lex->GetBase();
	
							p.first = Corpus::CWordDictionary::GetInstance()->Add(s);
							p.second = str_msk;
	
							if(COperator::collectNames)
							{
								std::vector<Corpus::DLONG>::iterator location;
								location = find(values->begin(),values->end(),p.first);
								if (location == values->end())
									values->push_back(p.first);
							}
	
							val.push_back(p);
						}
						else
						{
							val.push_back(Corpus::CValue(0,0));
						}
					}
				}
			}//if(Corpus::CToken::IsStrAttribute(attIndex))
			else
			{
				std::vector<Corpus::DLONG>::iterator location;
				if(attIndex == Corpus::CToken::flex)
				{
					p.first = (*token)[attIndex];
					if(COperator::collectNames)
					{
						location = find(values->begin(),values->end(),p.first);
						if (location == values->end())
							values->push_back(p.first);
					}
				}
				else// attIndex == Corpus::CToken::pos
				{
					p.second = (*token)[attIndex];
					if(COperator::collectNames)
					{
						location = find(values->begin(),values->end(),p.second);
						if (location == values->end())
							values->push_back(p.second);
					}
				}
				val.push_back(p);
				score = false;
			}

			return score;
		}

		void CSimpleOperator::GetValueNames(std::stringstream& out)
		{
			hadZeroDuringValuesGet = false;
			if(values->empty())
			{
				out << "0";
				hadZeroDuringValuesGet = true;
				return;
			}

			out << (*values)[0];
			if( (*values)[0] == 0)
				hadZeroDuringValuesGet = true;

			for(int i = 1; i < (int)values->size(); i++)
			{
				if( (*values)[i] == 0)
					hadZeroDuringValuesGet = true;
				out << "," << (*values)[i];
			}

			if(!hadZeroDuringValuesGet)
			{
				out << ",0";
				hadZeroDuringValuesGet = true;
			}
		}

		std::string CSimpleOperator::ToString()
		{
			std::stringstream ss;

			ss << Corpus::CTag::GetAttribString(attIndex) << "[" << position->ToString() << "]";

			return ss.str();;
		}
	}
}

