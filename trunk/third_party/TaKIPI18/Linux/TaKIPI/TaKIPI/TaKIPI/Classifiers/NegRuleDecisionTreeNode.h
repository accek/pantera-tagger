/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_NEGRULEDESICIONTREENODE_H
#define TAKIPI_NEGRULEDESICIONTREENODE_H

#include "../../takipi_headers.h"

namespace TaKIPI
{
	namespace Classifiers
	{

		class TAKIPI_DEV_API CNegRuleDecisionTreeNode
		{
		public:
			CNegRuleDecisionTreeNode(std::vector<std::string>& tree);

			CNegRuleDecisionTreeNode();
			
			

			// zwraca licznosc danego wezla/liscia
			unsigned int GetCount()
			{
				return count;
			}

			//zwraca prawdopobodienstwo wystapienia danego wezla/liscia
			double GetProbability()
			{
				return probability;
			}

			//wypelnia wartosci prawdopodobienstw
			void UpdateProbabilities(unsigned int amount, double lambda, int possibleTags);

			// Serializacja i deserializacja wêz³a.
			void Serialize(std::ofstream& file);
			static CNegRuleDecisionTreeNode* Deserialize(std::ifstream& file);

			void UseNegRuleTree(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& sentence, Corpus::CValue& mask);

			~CNegRuleDecisionTreeNode(void);

			static Corpus::DLONG statystyka;
		private:
			// czy wêze³, czy mo¿e liœæ
			bool isNode;
			// ile przypadków wyl¹dowa³o w danym wêŸle
			unsigned int count;
			// ile przypadków b³êdnych wyl¹dowa³o w danym wêŸle
			unsigned int wCount;
			// Statystyka wystapienia danego liscia/wezla (znormalizowana)
			double probability;
			// Statystyka dla pozosta³ych nietrafionych lexemów.
			double wProbability;
			// Statystyka wystapienia danego liscia/wezla (znormalizowana i zlogarytmizowana)
			double lProbability;
			// Statystyka dla pozosta³ych nietrafionych lexemów.
			double lWrongProbability;
			// atrybut podawanego rekordu ucz¹cego
			int attribute;
			// wartoœæ danego atrybutu
			// (indeks s³ownika dla stringa/wartoœæ liczbowa-biarna dla atrybutu)
			Corpus::DLONG decision;
			// mapa krawêdzi wychodz¹cych z drzewa
			std::map<Corpus::CValue,CNegRuleDecisionTreeNode*> *edges;

			//jesli jest lisciem to odczytujemy jego dane
			void GetLeafInfo(std::string& leafInfo);
			//buduje krawedzie
			void BuildEdges(std::vector<std::string>& tree);
			// usuwa znak '|' z pocz¹tku lini i zwraca iloœæ podga³êzi
			void ParseBranches(std::vector<std::string>& tree);
			void AddEdge(std::vector<std::string>& edge);


			CNegRuleDecisionTreeNode(const CNegRuleDecisionTreeNode&);//no implementation
			CNegRuleDecisionTreeNode& operator =(CNegRuleDecisionTreeNode&); //no implementation
		};
	}// namespace Classifiers
}//namespace TaKIPI
#endif//TAKIPI_NEGRULEDESICIONTREENODE_H

