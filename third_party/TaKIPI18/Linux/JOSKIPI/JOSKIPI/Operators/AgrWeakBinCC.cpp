#include "../joskipi_headers.h"
#include "AgrWeakBinCC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CAgrWeakBinCC::CAgrWeakBinCC(std::vector<pos_ptr>* positions,CConstValue *atrSet, int bits) throw (std::string)
			:CBinaryComplexC(positions,0),attributes(atrSet)
		{
			if(bits < 0)
				throw std::string("Illegal number of bits.");

			this->bits = bits;
		}

		CAgrWeakBinCC::~CAgrWeakBinCC(void)
		{
		}

		bool CAgrWeakBinCC::FindAgrPaths(Corpus::CSentence &sentence, std::vector< std::set< Corpus::CValue > > &paths,
			std::vector< Corpus::CValue > &mask, int s_pos, int e_pos, int min_bits)
		{
			
			// paths: a vector of CValue sets, representing all possible paths for a an agreement to hold
			{
				// take only fully specified tags from begTok
				Corpus::CToken *begTok = sentence[sentence.GetPosition()+s_pos];
				std::set< Corpus::CValue > here;
				
				for(int lexnum = 0; lexnum < (int)begTok->GetLexCount(); lexnum++)
				{
					Corpus::DLONG flexClass = mask[0].first & begTok->GetLexem(lexnum)->GetTag().FlexClass;
					Corpus::DLONG gramCat = mask[0].second & begTok->GetLexem(lexnum)->GetTag().Categories;
				
					if(Corpus::Parser::BitCount(flexClass) + Corpus::Parser::BitCount(gramCat) >= min_bits)
					{
						here.insert(Corpus::CValue(flexClass,gramCat));
					}
				}
				if(here.size() == 0)
				{
					// only underspecified tags at beg
					return false;
				}
				paths.push_back(here);
			}
			for(int toknum = s_pos + 1; toknum <= e_pos - 1; toknum++)
			{
				// take only specified tags
				Corpus::CToken *currTok = sentence[sentence.GetPosition()+toknum];
				std::set< Corpus::CValue > here;
				bool underspecified = false;
				
				for(int lexnum = 0; lexnum < (int)currTok->GetLexCount(); lexnum++)
				{
					Corpus::DLONG flexClass = mask[0].first & currTok->GetLexem(lexnum)->GetTag().FlexClass;
					Corpus::DLONG gramCat = mask[0].second & currTok->GetLexem(lexnum)->GetTag().Categories;
					here.insert(Corpus::CValue(flexClass,gramCat));
					if(flexClass == 0 && gramCat == 0)
					{
						// no need to proceed further, this token will be skipped anyway
						underspecified = true;
						break;
					}
				}
				if(!underspecified)
				{
					// take if any specified tags present
					paths.push_back(here);
				}
			}
			{
				// take only specified tags from endTok
				Corpus::CToken *endTok = sentence[sentence.GetPosition()+e_pos];
				std::set< Corpus::CValue > here;
				
				for(int lexnum = 0; lexnum < (int)endTok->GetLexCount(); lexnum++)
				{
					Corpus::DLONG flexClass = mask[0].first & endTok->GetLexem(lexnum)->GetTag().FlexClass;
					Corpus::DLONG gramCat = mask[0].second & endTok->GetLexem(lexnum)->GetTag().Categories;
					if(Corpus::Parser::BitCount(flexClass) + Corpus::Parser::BitCount(gramCat) >= min_bits)
					{
						here.insert(Corpus::CValue(flexClass,gramCat));
					}
				}
				if(here.size() == 0)
				{
					// only underspecified tags at end
					return false;
				}
				paths.push_back(here);
			}
			
			// now make an O(n2) search within remaining positions
			for(int pos_start = 0; pos_start < (int)paths.size(); pos_start++)
			{
				for(int pos = 0; pos < (int)paths.size(); pos++)
				{
					if(pos == pos_start)
						continue;
					Constrain(paths[pos], paths[pos_start]);
					if(paths[pos].size() == 0)
						return false;
				}
			}
			
			return true;
		}
		
		bool CAgrWeakBinCC::Evaluate(Corpus::CSentence &sentence)
		{
			Corpus::CPosition s, e;
			CalculateLimits(sentence,s,e);
			
			int size = e.GetPos() - s.GetPos() + 1;
			if(size < 1)
			{
				return false;
			}
			
			std::vector< std::set< Corpus::CValue > > paths;
			std::vector< Corpus::CValue > val;
			attributes->GetValue(val,sentence);
			
			return FindAgrPaths(sentence, paths, val, s.GetPos(), e.GetPos(), this->bits);
		}
		
		void CAgrWeakBinCC::Constrain(std::set<Corpus::CValue> &what, std::set<Corpus::CValue> &constr)
		{
			for(std::set<Corpus::CValue>::iterator atwhat = what.begin(); atwhat != what.end();)
			{
				bool satisfied = false;
				for(std::set<Corpus::CValue>::iterator atconstr = constr.begin(); atconstr != constr.end(); ++atconstr)
				{
					// if there is a common intersection, proceed with next
					if(ValuesInter(*atconstr, *atwhat))
					{
						satisfied = true;
						break;
					}
				}
				if(satisfied)
				{
					++atwhat;
				}
				else
				{
					// remove atwhat
					what.erase(atwhat++);
				}
			}
		}
		
		bool CAgrWeakBinCC::ValuesInter(const Corpus::CValue &first, const Corpus::CValue &second)
		{
			Corpus::DLONG isect1 = (first.first & second.first);
			Corpus::DLONG isect2 = (first.second & second.second);
			if(isect1 != first.first && isect1 != second.first)
				return false;
			if(isect2 != first.second && isect2 != second.second)
				return false;
			return true;
		}

		std::string CAgrWeakBinCC::ToString()
		{
			std::stringstream ss;
			using Corpus::CPosition;

			ss << "wagr(";

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

			ss << "," << attributes->ToString()
				<< "," << bits << ")";

			return ss.str();
		}
	}
}
