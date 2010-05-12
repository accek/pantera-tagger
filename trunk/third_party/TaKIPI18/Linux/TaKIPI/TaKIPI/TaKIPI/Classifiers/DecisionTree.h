/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_DECISIONTREE_H
#define TAKIPI_DECISIONTREE_H

#include "../../takipi_headers.h"
#include "Classifier.h"
#include "DecisionTreeNode.h"


namespace TaKIPI
{
	namespace Classifiers
	{
		class TAKIPI_DEV_API CDecisionTree : public CClassifier
		{
		public:
			CDecisionTree(std::vector<std::string>& drzewo, Corpus::TFlexClass flexClass, Corpus::DLONG gramCat);
			CDecisionTree(){};
			~CDecisionTree(void);

			// Statyczna metoda tworz�ca drzewo na stosie, z pliku QuinLana.
			static CDecisionTree* CreateTree(std::ifstream& file, bool simplyfied, 
				Corpus::TFlexClass flexClass, Corpus::DLONG gramCat);

			// Uzupe�nia statystyki zdania w oparciu o dane pobrane z drzewa.
			Corpus::CToken* Unify(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& srcSentence, TaKIPI::CLayers& layers);

			// Serializacja i deserializacja drzewa.
			void Serialize();
			static CDecisionTree* Deserialize(std::ifstream &file);

#ifdef OCR
			double meanProb;
#endif

		protected:
			// korze� drzewa decyzyjnego
			CDecisionTreeNode* root;

			// Klasa fleksyjna.
			Corpus::TFlexClass flexClass;
			// Kategoria gramatyczna.
			Corpus::DLONG gramaticalCat;

		};
	}// namespace Classifiers
}//namespace TaKIPI

#endif // TAKIPI_DECISIONTREE_H

