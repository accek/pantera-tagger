/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_BAGCLASSIFIER_H
#define TAKIPI_BAGCLASSIFIER_H

#include "../../takipi_headers.h"
#include "Classifier.h"


namespace TaKIPI
{
	namespace Classifiers
	{

		class TAKIPI_DEV_API CBagClassifier :
			public CClassifier
		{
		public:
			CBagClassifier(CClassifier* firstClassifier, JOSKIPI::Patterns::CPattern* firstPattern);
			/**
			Usuwa klasyfikatory i wzorce
			*/
			~CBagClassifier(void);
			Corpus::CToken* Unify(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& srcSentence, TaKIPI::CLayers& layers);
			void Add(CClassifier* classifier, JOSKIPI::Patterns::CPattern* pattern);

		protected:
			std::vector<CClassifier*> * baggedClassifiers;
			std::vector<JOSKIPI::Patterns::CPattern*>* baggedPatterns;
		private:
			CBagClassifier(const CBagClassifier&);// no implementation
			CBagClassifier& operator =(CBagClassifier&);// no implementation
		};
	}// namespace Classifiers
}//namespace TaKIPI

#endif //TAKIPI_BAGCLASSIFIER_H

