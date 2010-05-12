#include "../joskipi_headers.h"
#include "BinaryComplexC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CBinaryComplexC::CBinaryComplexC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions) throw(std::string)
			:CComplexCond(positions,conditions)
		{
			if (!positions || (int)positions->size() != 3)
				throw std::string("Null point vector (position) or wrong positions number.");
			startPos =  (*positions)[0];
			endPos =	(*positions)[1];
			varPos =	(*positions)[2];
		}

		CBinaryComplexC::~CBinaryComplexC(void)
		{
		}

		void CBinaryComplexC::CalculatePositionLimits(Corpus::CSentence &sentence,
			Corpus::CPosition &s, Corpus::CPosition &e, const Corpus::CPosition &initS, const Corpus::CPosition &initE)
		{
			if(initS.GetPos() == Corpus::CPosition::beginning)
			{
				s.SetPos(-sentence.GetPosition());
			}
			else if(initS.GetPos() == Corpus::CPosition::end)
			{
				s.SetPos(sentence.Size() - sentence.GetPosition() - 1);
			}
			else
			{
				s.SetPos(initS.GetPos());
			}

			if(initE.GetPos() == Corpus::CPosition::end)
			{
				e.SetPos(sentence.Size() - sentence.GetPosition() - 1);
			}
			else if(initE.GetPos() == Corpus::CPosition::beginning)
			{
				e.SetPos(-sentence.GetPosition());
			}
			else
			{
				e.SetPos(initE.GetPos());
			}
		}
		
		void CBinaryComplexC::CalculateLimits(Corpus::CSentence &sentence,
			Corpus::CPosition &s, Corpus::CPosition &e)
		{
			CalculatePositionLimits(sentence, s, e, *this->startPos, *this->endPos);
		}

	}
}

