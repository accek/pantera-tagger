/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_CLASSIFIERRULE_H
#define TAKIPI_CLASSIFIERRULE_H

#include "../../takipi_headers.h"
#include "../Layers.h"

namespace TaKIPI
{
	namespace Classifiers
	{

		class TAKIPI_DEV_API CClassifierRule
		{
		public:
			CClassifierRule(std::string ruleDataString);
			CClassifierRule(const CClassifierRule& cr);
			CClassifierRule& operator =(const CClassifierRule&); 
			~CClassifierRule(void);
			//czy regula pokrywa przyklad /pattern/
			bool Covers(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& srcSen,TaKIPI::CLayers& layers, 
				std::map<Corpus::DLONG,Corpus::CValue*> &cachedValues);

			double GetLProbability()
			{
				return lProbability;
			}
			double GetLWrongProbability()
			{
				return lwProbability;
			}
			Corpus::DLONG GetDecision()
			{
				return decision;
			}
			double GetGoodCount()
			{
				return goodCount;
			}
			std::string ToString();
			void UpdateProbabilities(double amount, double lambda, int possibleTags);

		private:
			std::vector<std::pair<Corpus::DLONG,Corpus::DLONG> > *antecedents;
			double lProbability;
			double lwProbability;
			Corpus::DLONG decision;
			double goodCount;
			double wrongCount;

			

		};

	}// namespace Classifiers
}//namespace TaKIPI


#endif //TAKIPI_CLASSIFIERRULE_H

