#include "../../takipi_headers.h"
#include "ClassifierRule.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		CClassifierRule::CClassifierRule(const CClassifierRule& cr)
		{
			antecedents = new std::vector<std::pair<Corpus::DLONG,Corpus::DLONG> >(*cr.antecedents);
			lProbability = cr.lProbability;
			lwProbability = cr.lwProbability;
			decision = cr.decision;;
			goodCount = cr.goodCount;
			wrongCount = cr.wrongCount;
		}

		CClassifierRule& CClassifierRule::operator =(const CClassifierRule& cr)
		{
			antecedents = cr.antecedents;
			lProbability = cr.lProbability;
			lwProbability = cr.lwProbability;
			decision = cr.decision;;
			goodCount = cr.goodCount;
			wrongCount = cr.wrongCount;
			return *this;
		}

		CClassifierRule::CClassifierRule(std::string ruleDataString)
		{
			antecedents = new std::vector<std::pair<Corpus::DLONG,Corpus::DLONG> >();

			std::vector<std::string> tokens;
			Corpus::Parser::Tokenize(ruleDataString,tokens,"()");
			for(size_t i = 0; i < tokens.size() - 2; i+=2)
			{
				std::vector<std::string> pairAttribValue;
				Corpus::Parser::Tokenize(tokens[i],pairAttribValue,"=");
				
				Corpus::DLONG attribute = Corpus::Parser::ParseDLONG(pairAttribValue[0]);
				Corpus::DLONG value = Corpus::Parser::ParseDLONG(pairAttribValue[1]);
				antecedents->push_back(std::pair<Corpus::DLONG,Corpus::DLONG>(attribute,value));
			}
			std::vector<std::string> temp;
			Corpus::Parser::Tokenize(tokens[tokens.size() - 2],temp,"=");
			if(temp[0].find("class") == std::string::npos)
			{
				throw Corpus::tagexception("CClassifierRule::CClassifierRule - oczekiwano class=xxxxx");
			}
			decision = Corpus::Parser::ParseDLONG(temp[1]);
			Corpus::Parser::Tokenize(tokens[tokens.size() - 1],temp,"/");
			goodCount = atof(temp[0].c_str());
			wrongCount = atof(temp[1].c_str());
		}

		CClassifierRule::~CClassifierRule(void)
		{
			if(antecedents)
			{
				delete antecedents;
				antecedents = NULL;
			}
		}

		std::string CClassifierRule::ToString()
		{
			std::stringstream ss;
			std::vector< std::pair <Corpus::DLONG,Corpus::DLONG> >::iterator it = antecedents->begin();
			if(it != antecedents->end())
			{
				ss << "(" << it->first << " = " << it->second << ")";
				it++;
			}
			
			ss << " => class=" << decision;
			return ss.str();
		}

		//czy regula pokrywa przyklad /pattern/
		bool CClassifierRule::Covers(JOSKIPI::Patterns::CPattern& pattern,Corpus::CSentence& srcSen, TaKIPI::CLayers& layers, 
			std::map<Corpus::DLONG,Corpus::CValue*> &cachedValues)
		{
			std::vector< std::pair <Corpus::DLONG,Corpus::DLONG> >::iterator it = antecedents->begin();
			bool covers = true;

#ifdef _DEBUG
			std::string s= this->ToString();
#endif
			while(it != antecedents->end() && covers)
			{
				Corpus::CValue* value;
				value = cachedValues[it->first];
				if(value == NULL)
				{
					Corpus::CValue val;
					pattern.GetValueAt((int)it->first,srcSen,(int)it->first,val);
					value = new Corpus::CValue(val);
					cachedValues[it->first] = value;
				}
				covers = ((value->first==0)?value->second:value->first) == it->second;
				it++;
			}
			return covers;
		}

		void CClassifierRule::UpdateProbabilities(double amount, double lambda, int possibleTags)
		{
			this->lProbability = log((goodCount + lambda)/(amount + (double)possibleTags*lambda));
			this->lwProbability = log((lambda)/(amount + (double)possibleTags*lambda));
			//this->lProbability = log((goodCount + lambda)/(goodCount + wrongCount + lambda*possibleTags));
			//this->lwProbability = log((wrongCount + lambda)/(goodCount + wrongCount + lambda*possibleTags));
		}

	}// namespace Classifiers
}//namespace TaKIPI

