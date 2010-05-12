/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_VOTINGWTACLASSIFIER_H
#define TAKIPI_VOTINGWTACLASSIFIER_H

#include "../../takipi_headers.h"
#include "BagClassifier.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		class TAKIPI_DEV_API CVotingWTAClassifier : public CBagClassifier
		{
		public:
			CVotingWTAClassifier(CClassifier * firstClassifier, JOSKIPI::Patterns::CPattern* firstPattern);
			Corpus::CToken* Unify(JOSKIPI::Patterns::CPattern& pattern,Corpus::CSentence& srcSentence, TaKIPI::CLayers& layers);
			~CVotingWTAClassifier(void);
		};
	}// namespace Classifiers
}//namespace TaKIPI
#endif // TAKIPI_VOTINGWTACLASSIFIER_H

