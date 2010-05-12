/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_CLASSIFIER_H
#define TAKIPI_CLASSIFIER_H

#include "../../takipi_headers.h"
#include "../Layers.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		class TAKIPI_DEV_API CClassifier
		{
		public:

			CClassifier(void)
			{
			}

			virtual ~CClassifier(void)
			{
			}

			// Uzupełnia statystyki zdania w oparciu o dane pobrane z klasyfikatora.
			virtual Corpus::CToken* Unify(JOSKIPI::Patterns::CPattern& pattern, 
				Corpus::CSentence& srcSentence, TaKIPI::CLayers& layers) = 0;

		};
	}// namespace Classifiers
}//namespace TaKIPI

#endif//TAKIPI_CLASSIFIER_H

