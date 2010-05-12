#include "../../takipi_headers.h"
#include "BagClassifier.h"


namespace TaKIPI
{
	namespace Classifiers
	{
		CBagClassifier::CBagClassifier(CClassifier* firstClassifier, JOSKIPI::Patterns::CPattern* firstPattern)
		{
			baggedClassifiers = new std::vector<CClassifier*>();
			baggedPatterns = new std::vector<JOSKIPI::Patterns::CPattern*>();

			baggedClassifiers->push_back(firstClassifier);
			baggedPatterns->push_back(firstPattern);
		}

		CBagClassifier::~CBagClassifier(void)
		{
			if(baggedClassifiers)
			{
				std::vector<CClassifier *>::iterator it = baggedClassifiers->begin();
				while(it != baggedClassifiers->end())
				{
					delete *it;
					it++;
				}
				baggedClassifiers = NULL;
			}

			if(baggedPatterns)
			{

				std::vector<JOSKIPI::Patterns::CPattern*>::iterator it2 = baggedPatterns->begin();
				while(it2 != baggedPatterns->end())
				{
					delete *it2;
					it2++;
				}
				baggedPatterns = NULL;
			}
		}

		void CBagClassifier::Add(CClassifier* classifier, JOSKIPI::Patterns::CPattern* pattern)
		{
			baggedClassifiers->push_back(classifier);
			baggedPatterns->push_back(pattern);
		}

		Corpus::CToken* CBagClassifier::Unify(JOSKIPI::Patterns::CPattern& pattern,
			Corpus::CSentence& srcSentence, TaKIPI::CLayers& layers)
		{
			std::vector<CClassifier *>::iterator it = baggedClassifiers->begin();
			std::vector<JOSKIPI::Patterns::CPattern *>::iterator it2 = baggedPatterns->begin();
			while(it != baggedClassifiers->end())
			{
				(*it)->Unify(*(*it2),srcSentence,layers);
				it++;
				it2++;
			}

			return 0;
		}

	}// namespace Classifiers
}//namespace TaKIPI
