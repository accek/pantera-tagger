#include "../joskipi_headers.h"
#include "AgrPPBinCC.h"


namespace JOSKIPI
{
	namespace Operators
	{
		CAgrPPBinCC::CAgrPPBinCC(std::vector<pos_ptr>* positions,CConstValue* atrSet, int bits)
			:CBinaryComplexC(positions,0),attributes(atrSet)
		{
			this->bits = bits;
		}

		CAgrPPBinCC::~CAgrPPBinCC(void)
		{
			delete attributes;
		}

		bool CAgrPPBinCC::Evaluate(Corpus::CSentence& sentence)
		{
			using Corpus::CValue;
			using Corpus::CToken;
			using std::vector;
			using Corpus::CPosition;
			;
			pos_ptr p0 = this->startPos;
			pos_ptr p1 = this->endPos;

			if(p0->GetPos() + sentence.GetPosition() < 0
				|| p0->GetPos() + sentence.GetPosition() >= sentence.Size()
				|| p1->GetPos() + sentence.GetPosition() < 0
				|| p1->GetPos() + sentence.GetPosition() >= sentence.Size())
				return false;

			CToken* t0 = sentence[sentence.GetPosition() + p0->GetPos()];
			CToken* t1 = sentence[sentence.GetPosition() + p1->GetPos()];

			if(t0->GetLexCount() < t1->GetLexCount())
			{
				CToken* temp = t0;
				t0 = t1;
				t1 = temp;
			}

			vector<CValue > masks;
			CValue mask,maskedVal0,maskedVal1;
			attributes->GetValue(masks,sentence);
			mask = masks[0];

			for(int i = 0; i < (int)t0->GetLexCount(); i++)
			{
				t0->GetLexem(i)->GetMaskedValue(mask,maskedVal0);
				if(Corpus::Parser::BitCount(maskedVal0.first) + Corpus::Parser::BitCount(maskedVal0.second) < bits)
					continue;

				for(int j = 0; j < (int)t1->GetLexCount(); j++)
				{
					t1->GetLexem(j)->GetMaskedValue(mask,maskedVal1);
					if(Corpus::Parser::BitCount(maskedVal1.first) + Corpus::Parser::BitCount(maskedVal1.second) < bits)
						continue;

					if(Corpus::Parser::BitCount(maskedVal0.first & maskedVal1.first) 
						+ Corpus::Parser::BitCount(maskedVal0.second & maskedVal1.second) >= bits)
						return true;
				}
			}

			return false;
		}

		std::string CAgrPPBinCC::ToString()
		{
			std::stringstream ss;
			using Corpus::CPosition;

			ss << "agrpp(";

			if(startPos->GetPos() == CPosition::beginning)
				ss << "begin";
			else if(startPos->GetPos() == CPosition::end)
				ss << "end";
			else 
				ss << startPos->GetPos();

			ss << ",";

			if(endPos->GetPos() == CPosition::beginning)
				ss << "begin";
			else if(endPos->GetPos() == CPosition::end)
				ss << "end";
			else 
				ss << endPos->GetPos();

			ss << "," << attributes->ToString() << "," << bits << ")";

			return ss.str();

		}

	}
}
