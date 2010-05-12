/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
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

			// Serializacja i deserializacja w�z�a.
			void Serialize(std::ofstream& file);
			static CNegRuleDecisionTreeNode* Deserialize(std::ifstream& file);

			void UseNegRuleTree(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& sentence, Corpus::CValue& mask);

			~CNegRuleDecisionTreeNode(void);

			static Corpus::DLONG statystyka;
		private:
			// czy w�ze�, czy mo�e li��
			bool isNode;
			// ile przypadk�w wyl�dowa�o w danym w�le
			unsigned int count;
			// ile przypadk�w b��dnych wyl�dowa�o w danym w�le
			unsigned int wCount;
			// Statystyka wystapienia danego liscia/wezla (znormalizowana)
			double probability;
			// Statystyka dla pozosta�ych nietrafionych lexem�w.
			double wProbability;
			// Statystyka wystapienia danego liscia/wezla (znormalizowana i zlogarytmizowana)
			double lProbability;
			// Statystyka dla pozosta�ych nietrafionych lexem�w.
			double lWrongProbability;
			// atrybut podawanego rekordu ucz�cego
			int attribute;
			// warto�� danego atrybutu
			// (indeks s�ownika dla stringa/warto�� liczbowa-biarna dla atrybutu)
			Corpus::DLONG decision;
			// mapa kraw�dzi wychodz�cych z drzewa
			std::map<Corpus::CValue,CNegRuleDecisionTreeNode*> *edges;

			//jesli jest lisciem to odczytujemy jego dane
			void GetLeafInfo(std::string& leafInfo);
			//buduje krawedzie
			void BuildEdges(std::vector<std::string>& tree);
			// usuwa znak '|' z pocz�tku lini i zwraca ilo�� podga��zi
			void ParseBranches(std::vector<std::string>& tree);
			void AddEdge(std::vector<std::string>& edge);


			CNegRuleDecisionTreeNode(const CNegRuleDecisionTreeNode&);//no implementation
			CNegRuleDecisionTreeNode& operator =(CNegRuleDecisionTreeNode&); //no implementation
		};
	}// namespace Classifiers
}//namespace TaKIPI
#endif//TAKIPI_NEGRULEDESICIONTREENODE_H

