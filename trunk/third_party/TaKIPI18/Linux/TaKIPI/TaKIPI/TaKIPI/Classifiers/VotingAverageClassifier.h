/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_VOTINGAVERAGECLASSIFIER_H
#define TAKIPI_VOTINGAVERAGECLASSIFIER_H

#include "../../takipi_headers.h"
#include "BagClassifier.h"

namespace TaKIPI
{
	namespace Classifiers
	{

		class TAKIPI_DEV_API CVotingAverageClassifier :	public CBagClassifier
		{
		public:
			CVotingAverageClassifier(CClassifier * firstClassifier, JOSKIPI::Patterns::CPattern* firstPattern);
			Corpus::CToken* Unify(JOSKIPI::Patterns::CPattern& pattern,Corpus::CSentence& srcSentence, TaKIPI::CLayers& layers);
			~CVotingAverageClassifier(void);
		};
	}// namespace Classifiers
}//namespace TaKIPI
#endif // TAKIPI_VOTINGAVERAGECLASSIFIER_H

