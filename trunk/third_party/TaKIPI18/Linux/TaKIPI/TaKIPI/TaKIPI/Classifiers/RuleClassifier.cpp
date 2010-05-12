#include "../../takipi_headers.h"
#include "RuleClassifier.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		CRuleClassifier::CRuleClassifier(std::vector<std::string> & rulesData, Corpus::TFlexClass flexClass, 
			Corpus::DLONG gramCat)
		{
			rules = new std::vector< CClassifierRule >();

			double amount = 0.0;
			for(std::vector<std::string>::iterator it = rulesData.begin(); it != rulesData.end(); it++)
			{
				CClassifierRule rule = CClassifierRule(*it);
				rules->push_back(rule);
				amount += rule.GetGoodCount();
			}

			Corpus::CValue mask;
			int k = 1;
			for(int i = 2; i < (int)Corpus::CToken::END; i++)
			{
				Corpus::CTag::GetAttribMask(i,mask);
				if((mask.first & flexClass)!= 0)
					k *= Corpus::Parser::BitCount(mask.first & flexClass);
				if((mask.second & gramCat)!= 0)
					k *= Corpus::Parser::BitCount(mask.second & gramCat);
			}

			double lambda = ((double)(k - 1))/k;

			if(lambda == 0)
				lambda = 1;

			for(std::vector<CClassifierRule>::iterator itr = rules->begin(); itr != rules->end(); itr++)
			{
				itr->UpdateProbabilities(amount,lambda,k);
			}
		}

		Corpus::CToken* CRuleClassifier::Unify(JOSKIPI::Patterns::CPattern &pattern, Corpus::CSentence &srcSentence, 
			TaKIPI::CLayers &layers)
		{
			bool notFound = true;
			std::vector<CClassifierRule>::iterator it = rules->begin();
			std::map<Corpus::DLONG,Corpus::CValue*> cachedValues;
			while(it != rules->end() && notFound)
			{
				if(it->Covers(pattern,srcSentence,layers,cachedValues))
				{
					notFound = false;
					Corpus::CValue mask,temp;
					layers.GetLayerMask(mask);
					double newWProbability = it->GetLWrongProbability() - log((double)srcSentence.GetCurrentToken()->GetLexCount());
					double lProbability = it->GetLProbability();
					for(int i = 0; i < (int)srcSentence.GetCurrentToken()->GetLexCount(); i++)
					{
						(*(srcSentence.GetCurrentToken()->GetLexem(i))).GetMaskedValue(mask,temp);
						if(mask.second == 0)
						{
							if(temp.first == it->GetDecision())
								srcSentence.GetCurrentToken()->SetLexProbability(i, lProbability);
							else
								srcSentence.GetCurrentToken()->SetLexProbability(i, newWProbability);
						}
						else
						{
							if(temp.second == it->GetDecision())
								srcSentence.GetCurrentToken()->SetLexProbability(i, lProbability);
							else
								srcSentence.GetCurrentToken()->SetLexProbability(i, newWProbability);
						}
					}
				}
				it++;
			}
			std::map<Corpus::DLONG,Corpus::CValue*>::iterator itCache = cachedValues.begin();
			while(itCache != cachedValues.end())
			{
				delete itCache->second;
				itCache++;
			}
			if(notFound)
			{
				throw Corpus::tagexception((std::string("Ruleset does not cover pattern! ") + pattern.ToString()).c_str());
			}

			return 0;
		}

		CRuleClassifier::~CRuleClassifier(void)
		{
			if(rules)
			{
				delete rules;
				rules = NULL;
			}
		}

	}//namespace TaKIPI

}//namespace Classifiers
