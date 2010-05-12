#include "../joskipi_headers.h"
#include "FixSimpOP.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CFixSimpOP::CFixSimpOP(void)
		{
		}

		CFixSimpOP::~CFixSimpOP(void)
		{
		}

		CFixSimpOP::CFixSimpOP(pos_ptr position,int attributeIndex,int howMany) throw (std::string)
			:CSimpleOperator(position,attributeIndex)
		{
			if (!Corpus::CToken::IsStrAttribute(attributeIndex))
				throw std::string("Value of attribute is not stirng type.");

			this->howMany = howMany;
		}

		CFixSimpOP::CFixSimpOP(const CFixSimpOP& op) : CSimpleOperator( (const CSimpleOperator&) op)
		{
			this->howMany = op.howMany;
		}

		const CFixSimpOP& CFixSimpOP::operator =(const CFixSimpOP& op)
		{
			this->attIndex = op.attIndex;
			this->position = op.position;
			this->howMany = op.howMany;
			*(this->values) = *(op.values);
			return *this;
		}

		bool CFixSimpOP::GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence)
		{
			Corpus::CToken* token = sentence[sentence.GetPosition()+position->GetPos()];

			Corpus::CValue p(0,str_msk);
			val.clear();

			if (attIndex == Corpus::CToken::orth)
			{
				std::string s = token->GetOrth();

				if (abs(howMany) < (int)s.length())
				{	
					if (howMany >= 0)
					{
						s = s.substr(0,howMany);
					}
					else
					{
						s = s.substr(s.length()+howMany,-howMany);
					}
				}

				p.first = Corpus::CWordDictionary::GetInstance()->Add(s);

				if(COperator::collectNames)
				{
					std::vector<Corpus::DLONG>::iterator location;
					location = find(values->begin(),values->end(),p.first);
					if (location == values->end())
						values->push_back(p.first);
				}

				val.push_back(p);
			}
			else //if base
			{
				for(int i = 0; i < (int)token->GetLexCount(); i++)
				{
					std::string s = (*token->GetLexem(i)).GetBase();

					if (abs(howMany) < (int)s.length())
					{	
						if (howMany >= 0)
						{
							s = s.substr(0,howMany);
						}
						else
						{
							s = s.substr(s.length()+howMany,-howMany);
						}
					}

					p.first = Corpus::CWordDictionary::GetInstance()->Add(s);

					std::vector<Corpus::DLONG>::iterator location;
					location = find(values->begin(),values->end(),p.first);
					if (location == values->end())
						values->push_back(p.first);

					val.push_back(p);
				}
			}

			return true;
		}

		std::string CFixSimpOP::ToString()
		{
			//TODO: Dorobiæ.

			return std::string("TODO");
		}

	}
}
