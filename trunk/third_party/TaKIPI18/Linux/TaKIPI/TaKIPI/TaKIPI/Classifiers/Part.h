/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_PART_H
#define TAKIPI_PART_H

#include "../../takipi_headers.h"
#include "RuleClassifier.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		class TAKIPI_DEV_API CPart :
			public CRuleClassifier
		{
		public:
			CPart(std::vector<std::string> & rulesData, Corpus::TFlexClass flexClass, Corpus::DLONG gramCat);
			~CPart(void);
			static CPart* CreateClassifier(std::ifstream& inFile, Corpus::TFlexClass flexClass, Corpus::DLONG gramCat);
		};

	}// namespace Classifiers
}//namespace TaKIPI


#endif //TAKIPI_PART_H

