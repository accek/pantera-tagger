/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_RULECLASSIFIER_H
#define TAKIPI_RULECLASSIFIER_H

#include "../../takipi_headers.h"
#include "Classifier.h"
#include "ClassifierRule.h"

namespace TaKIPI
{
	namespace Classifiers
	{

		class TAKIPI_DEV_API CRuleClassifier :	public CClassifier
		{
		public:
			CRuleClassifier(std::vector<std::string> & rulesData, Corpus::TFlexClass flexClass, Corpus::DLONG gramCat);
			virtual ~CRuleClassifier(void);
			Corpus::CToken* Unify(JOSKIPI::Patterns::CPattern &pattern, Corpus::CSentence &srcSentence, TaKIPI::CLayers &layers);
		protected:
			std::vector< CClassifierRule > *rules;

		private:
			CRuleClassifier(const CRuleClassifier&);// no implementation
			CRuleClassifier& operator=(CRuleClassifier&);// no implementation

		};
	}// namespace Classifiers
}//namespace TaKIPI


#endif // TAKIPI_RULECLASSIFIER_H

