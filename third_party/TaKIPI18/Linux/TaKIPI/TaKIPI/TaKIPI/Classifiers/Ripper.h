/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_RIPPER_H
#define TAKIPI_RIPPER_H

#include "../../takipi_headers.h"
#include "RuleClassifier.h"

namespace TaKIPI
{
	namespace Classifiers
	{

		class TAKIPI_DEV_API CRipper :	public CRuleClassifier
		{
		public:
			CRipper(std::vector<std::string> & rulesData, Corpus::TFlexClass flexClass, Corpus::DLONG gramCat);	
			~CRipper(void);
			static CRipper* CreateClassifier(std::ifstream& file, Corpus::TFlexClass flexClass, Corpus::DLONG gramCat);
		private:
			CRipper(const CRipper&);// no implementation
			CRipper& operator=(CRipper&);// no implementation
		};
	}// namespace Classifiers
}//namespace TaKIPI


#endif //TAKIPI_RIPPER_H

