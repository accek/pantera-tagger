/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc≈Çawskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardy≈Ñski
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_LMTREENODE_H
#define TAKIPI_LMTREENODE_H

#include "../../takipi_headers.h"
#include "LogisticModel.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		class TAKIPI_DEV_API CLMTreeNode
		{
		public:
			CLMTreeNode(std::vector<std::string>& treedata, CLogisticModel * models);
			~CLMTreeNode(void);
			Corpus::CToken* Unify(JOSKIPI::Patterns::CPattern& pattern,Corpus::CSentence& srcSen,TaKIPI::CLayers& layers);
		private:
			CLogisticModel* model;
			int attribute;
			std::map<Corpus::CValue,CLMTreeNode*> *edges;

			//jesli jest lisciem to odczytujemy jego dane
			void GetLeafInfo(std::string& leafInfo, CLogisticModel * models);
			//buduje krawedzie
			void BuildEdges(std::vector<std::string>& tree, CLogisticModel *models);
			// usuwa znak '|' z poczπtku lini i zwraca iloúÊ podga≥Ízi
			void ParseBranches(std::vector<std::string>& tree);
			void AddEdge(std::vector<std::string>& edge, CLogisticModel *models);

			CLMTreeNode(const CLMTreeNode&);//no implementation
			CLMTreeNode& operator=(CLMTreeNode&);//no implementation
		};
	}// namespace Classifiers
}//namespace TaKIPI

#endif //TAKIPI_LMTREENODE_H

