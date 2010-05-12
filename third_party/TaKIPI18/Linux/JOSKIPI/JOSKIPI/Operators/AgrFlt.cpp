#include "../joskipi_headers.h"
#include "AgrFlt.h"
#include "AgrWeakBinCC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CAgrFlt::CAgrFlt(pos_ptr startPos, pos_ptr endPos, CConstValue *agrAttrs, int agrBits, CConstValue *fltAttrs) throw (std::string)
			:COperator(startPos), endPos(endPos), agrAttribs(agrAttrs), fltAttribs(fltAttrs)
		{
			if(agrBits < 0)
				throw std::string("Illegal number of bits.");

			this->bits = agrBits;
			values = new std::vector<Corpus::DLONG>();
		}
		
		CAgrFlt::~CAgrFlt()
		{
			delete values;
			delete agrAttribs;
			delete fltAttribs;
		}
		
		Corpus::CValue CAgrFlt::MaskedValues(std::set<Corpus::CValue> &where, const Corpus::CValue &mask)
		{
			Corpus::CValue out(0,0);
			for(std::set<Corpus::CValue>::iterator atwhere = where.begin(); atwhere != where.end(); ++atwhere)
			{
				out.first |= atwhere->first & mask.first;
				out.second |= atwhere->second & mask.second;
			}
			return out;
		}
		
		bool CAgrFlt::GetValue(std::vector<Corpus::CValue > &val, Corpus::CSentence& sentence)
		{
			val.clear();
			if(sentence.GetPosition() == Corpus::CPosition::nowhere
				|| position->GetPos() == Corpus::CPosition::nowhere
				|| endPos->GetPos() == Corpus::CPosition::nowhere)
			{
				val.push_back(Corpus::CValue(0,0));
				return false;
			}
			
			// calculate sentence limits
			Corpus::CPosition start, end;
			CBinaryComplexC::CalculatePositionLimits(sentence, start, end, *position, *endPos);
			
			std::vector<Corpus::CValue> maskVec;
			fltAttribs->GetValue(maskVec,sentence);
			
			int size = end.GetPos() - start.GetPos() + 1;
			if(size < 1)
			{
				val.push_back(Corpus::CValue(0,0));
				return false;
			}
			
			// prepare for agreement calculation
			std::vector< std::set< Corpus::CValue > > paths;
			std::vector< Corpus::CValue > mask;
			agrAttribs->GetValue(mask, sentence);

			// calculate agreement
			bool hasAgreed = CAgrWeakBinCC::FindAgrPaths(sentence, paths, mask, start.GetPos(), end.GetPos(), bits);
			if(!hasAgreed)
			{
				val.push_back(Corpus::CValue(0,0));
				return false;
			}
			
			// filter agreement values
			Corpus::CValue out(0,0);
			for(int pos = 0; pos < (int)paths.size(); pos++)
			{
				Corpus::CValue atpos(MaskedValues(paths[pos], maskVec[0]));
				out.first |= atpos.first;
				out.second |= atpos.second;
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
		
		void CAgrFlt::GetValueNames(std::stringstream &out)
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
		
		std::string CAgrFlt::ToString()
		{
			std::stringstream ss;

			ss << "agrflt(";
			if(position->GetPos() == Corpus::CPosition::beginning)
				ss << "begin";
			else if(position->GetPos() == Corpus::CPosition::end)
				ss << "end";
			else 
				ss << position->GetPos();
			ss << ",";
			if(endPos->GetPos() == Corpus::CPosition::beginning)
				ss << "begin";
			else if(endPos->GetPos() == Corpus::CPosition::end)
				ss << "end";
			else 
				ss << endPos->GetPos();
			
			ss << "," << agrAttribs->ToString()
					<< "," << bits
					<< "," << fltAttribs->ToString()
					<< ")";

			return ss.str();
		}
	}
}
