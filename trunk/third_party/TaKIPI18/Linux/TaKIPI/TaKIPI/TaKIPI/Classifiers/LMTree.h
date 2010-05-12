/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc≈Çawskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardy≈Ñski
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_LMTREE_H
#define TAKIPI_LMTREE_H

#include "../../takipi_headers.h"
#include "Classifier.h"
#include "LMTreeNode.h"

namespace TaKIPI
{
	namespace Classifiers
	{

		class TAKIPI_DEV_API CLMTree : public CClassifier
		{
		public:
			CLMTree(std::vector<std::string>& treedata, std::vector< std::vector<std::string> > & models);
			virtual ~CLMTree(void);
			//Tworzenie drzewa przez wczytanie z pliku
			static CLMTree* CreateTree(std::ifstream& inFile);

			virtual Corpus::CToken* Unify(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& srcSentence, TaKIPI::CLayers& layers);

			//TODO: (de)serializacja
		private:
			//korzeÒ drzewa
			CLMTreeNode* root;
			//modele w liúciach
			CLogisticModel * models;
		};
	}// namespace Classifiers
}//namespace TaKIPI

#endif//TAKIPI_LMTREE_H

