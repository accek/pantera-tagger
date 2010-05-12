#include "../joskipi_headers.h"
#include "AgrBinCC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		CAgrBinCC::CAgrBinCC(std::vector<pos_ptr>* positions,CConstValue* atrSet, int bits) throw (std::string)
			:CBinaryComplexC(positions,0),attributes(atrSet)
		{
			if(bits < 0)
				throw std::string("Illegal number of bits.");

			this->bits = bits;
		}

		CAgrBinCC::~CAgrBinCC(void)
		{

		}

		bool CAgrBinCC::Evaluate(Corpus::CSentence& sentence)
		{
			using std::vector;
			using std::pair;
			using namespace Corpus;
			using Corpus::CPosition;
			//1
			Corpus::CPosition s, e;
			CalculateLimits(sentence,s,e);

			//2
			int size = e.GetPos() - s.GetPos() + 1;
			if(size < 1)
				return false;
			
			vector<vector<pair<CValue,bool> > > vecTab;

			//3
			vector<CValue > val;
			DLONG flexClass, gramCat;
			for(int i = s.GetPos(); i <= e.GetPos(); i++)
			{
				CToken* currTok = sentence[sentence.GetPosition()+i];
				vector<pair<CValue,bool> > vecTemp;

				//3a
				for(int j = 0; j < (int)currTok->GetLexCount(); j++)
				{
					attributes->GetValue(val,sentence);
					flexClass = val[0].first & currTok->GetLexem(j)->GetTag().FlexClass;
					gramCat = val[0].second & currTok->GetLexem(j)->GetTag().Categories;

					if(flexClass == 0 && gramCat == 0)
						continue;

					bool contains = false;

					for(vector<pair<CValue,bool> >::iterator iter = vecTemp.begin();
						iter != vecTemp.end(); iter++)
					{
						if(iter->first.first == flexClass 
							&& iter->first.second == gramCat)
						{
							contains = true;
							break;
						}
					}
					if(!contains)
					{
						//10
						vecTemp.push_back(pair<CValue,bool>(
							CValue(flexClass,gramCat),false));
					}
				}
				//7
				if((int)vecTemp.size() == 0)
				{
					return false;
				}
				else
					vecTab.push_back(vecTemp);
			}
			// teraz vecTab to wektor zawierający zbiory wartości rzutowanych na maskę, każdej wartości towarzyszy false
			//4,5,6
			vector<bool> procVarsTab;
			int varA = 0,
				varB = INT_MAX;
			for(int i = 0; i < (int)vecTab.size(); i++)
			{
				if(vecTab[varA].size() > (int)vecTab[i].size())
					varA = i;
				procVarsTab.push_back(false);
			}

			//8
			if(varA == INT_MAX)
				return false;
			procVarsTab[varA] = true;

			//10
			if((int)vecTab[varA].size() == 1)
			{
				return CheckAllForOne(vecTab,vecTab[varA][0].first);
			}

			while(true)
			{
				//11
				int iterI = 0;

				for(iterI = 0; iterI < (int)vecTab.size(); iterI++)
				{
					if(procVarsTab[iterI] == false)
					{
						varB = iterI;
						break;
					}
				}

				if(iterI == (int)vecTab.size())
				{
					return true;
				}

				for(int i = iterI; i < (int)vecTab.size(); i++)
				{
					if(procVarsTab[i] == false)
						if(vecTab[varB].size() > (int)vecTab[i].size())
							varB = i;
				}

				procVarsTab[varB] = true;

				//12
				for(int i = 0; i < (int)vecTab[varB].size(); i++)
				{
					vecTab[varB][i].second = false;
				}

				//13
				bool found = false;
				for(int i = 0; i < (int)vecTab[varA].size(); i++)
				{
					for(int j = 0; j < (int)vecTab[varB].size(); j++)
					{
						if(vecTab[varA][i].first.first == vecTab[varB][j].first.first
							&& vecTab[varA][i].first.second == vecTab[varB][j].first.second)
						{
							vecTab[varB][j].second = true;
						}
					}
				}

				//14
				vector<pair<CValue,bool> > temp;
				for(vector<pair<CValue,bool> >::iterator iter = vecTab[varB].begin();
					iter != vecTab[varB].end(); iter++)
				{
					if(iter->second == true)
						temp.push_back(*iter);
				}
				vecTab[varB].clear();
				for(int i = 0; i < (int)temp.size(); i++)
				{
					vecTab[varB].push_back(temp[i]);
				}

				//15
				if((int)vecTab[varB].size() == 0)
					return false;

				//16
				if((int)vecTab[varB].size() == 1)
				{
					return CheckAllForOne(vecTab,vecTab[varB][0].first);
				}

				//17,18
				found = false;
				for(int i = 0; i < (int)vecTab[varA].size(); i++)
				{
					vecTab[varA][i].second = false;
					for(int j = 0; j < (int)vecTab[varB].size(); j++)
					{
						if(vecTab[varA][i].first.first == vecTab[varB][j].first.first
							&& vecTab[varA][i].first.second == vecTab[varB][j].first.second)
						{
							vecTab[varA][i].second = true;
						}
					}
				}

				//19
				vector<pair<CValue,bool> > temp2;
				for(vector<pair<CValue,bool> >::iterator iter = vecTab[varA].begin();
					iter != vecTab[varA].end(); iter++)
				{
					if(iter->second == true)
						temp2.push_back(*iter);
				}
				vecTab[varA].clear();
				for(int i = 0; i < (int)temp2.size(); i++)
				{
					vecTab[varA].push_back(temp2[i]);
				}

				//20
				if((int)vecTab[varA].size() == 0)
					return false;

				//21
				if((int)vecTab[varA].size() == 1)
				{
					return CheckAllForOne(vecTab,vecTab[varA][0].first);
				}

				//22
				varA = varB;
			}
			
			return true;
		}

		bool CAgrBinCC::CheckAllForOne(std::vector<std::vector<std::pair<Corpus::CValue,bool> > >& vecTab,
			Corpus::CValue& val)
		{
			bool found = false;

			for(std::vector<std::vector<std::pair<Corpus::CValue,bool> > >::iterator i = vecTab.begin();
				i != vecTab.end(); i++)
			{
				found = false;
				for(std::vector<std::pair<Corpus::CValue,bool> >::iterator j = (*i).begin(); 
					j != (*i).end(); j++)
				{
					if( (*j).first.first == val.first && (*j).first.second == val.second )
					{
						found = true;
						break;
					}
				}
				if(found == false)
					return false;
			}

			return true;
		}

		std::string CAgrBinCC::ToString()
		{
			std::stringstream ss;
			using Corpus::CPosition;

			ss << "agr(";

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
