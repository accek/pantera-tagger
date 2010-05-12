/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc≈Çawskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardy≈Ñski
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_NEGRULEDECISIONTREE_H
#define TAKIPI_NEGRULEDECISIONTREE_H

#include "../../takipi_headers.h"
#include "NegRuleDecisionTreeNode.h"

namespace TaKIPI
{
	namespace Classifiers
	{

		class TAKIPI_DEV_API CNegRuleDecisionTree
		{
		public:
			CNegRuleDecisionTree(std::vector<std::string>& drzewo, Corpus::TFlexClass flexClass, Corpus::DLONG gramCat);
			CNegRuleDecisionTree();
			~CNegRuleDecisionTree(void);

			// Statyczna metoda tworzπca drzewo na stosie, z pliku QuinLana.
			static CNegRuleDecisionTree* CreateTree(std::ifstream& file, bool simplyfied, Corpus::TFlexClass flexClass, 
				Corpus::DLONG gramCat);
			

			void UseNegRuleTree(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& sentence);

			// Serializacja i deserializacja drzewa.
			void Serialize();
			static CNegRuleDecisionTree* Deserialize(std::ifstream &file);

		protected:
			// korzeÒ drzewa decyzyjnego
			CNegRuleDecisionTreeNode* root;

			// Klasa fleksyjna.
			Corpus::TFlexClass  flexClass;
			// Kategoria gramatyczna.
			Corpus::DLONG gramaticalCat;
		};
	}// namespace Classifiers
}//namespace TaKIPI
#endif//TAKIPI_NEGRULEDECISIONTREE_H

