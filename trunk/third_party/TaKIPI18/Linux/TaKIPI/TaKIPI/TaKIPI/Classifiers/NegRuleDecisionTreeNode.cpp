#include "../../takipi_headers.h"
#include "NegRuleDecisionTreeNode.h"


namespace TaKIPI
{
	namespace Classifiers
	{

		Corpus::DLONG CNegRuleDecisionTreeNode::statystyka = 0;

		CNegRuleDecisionTreeNode::CNegRuleDecisionTreeNode()
		{
			edges = new std::map<Corpus::CValue,CNegRuleDecisionTreeNode*> ();
		}

		CNegRuleDecisionTreeNode::CNegRuleDecisionTreeNode(std::vector<std::string>& tree)
		{
			edges = new std::map<Corpus::CValue,CNegRuleDecisionTreeNode*> ();
			std::string s = *tree.begin();

			if (s.find(":") == s.npos)
			{
				GetLeafInfo(s);
				return;
			}

			std::vector<std::string> temp;
			Corpus::Parser::Tokenize(s,temp);

			this->attribute = atoi((*temp.begin()).c_str());

			BuildEdges(tree);
		}

		CNegRuleDecisionTreeNode::~CNegRuleDecisionTreeNode(void)
		{
			if(edges)
			{
				CNegRuleDecisionTreeNode* tempEdge;
				for(std::map<Corpus::CValue,CNegRuleDecisionTreeNode*>::iterator i = edges->begin(); 
					i != edges->end(); i++)
				{
					tempEdge = (*i).second;
					delete tempEdge;
				}
				delete edges;
				edges = NULL;
			}
		}

		void CNegRuleDecisionTreeNode::BuildEdges(std::vector<std::string>& tree)
		{
			std::vector<std::string>::iterator iter = tree.begin();
			std::vector<std::string>::iterator i;

			//dla kazdej krawedzi budujemy na koncu niej wezel
			for(i = tree.begin(); i != tree.end(); i++)
			{
				if ( (int)(*i).length() == 0 )
					break;
				if ( (*i)[0] != '|')
				{
					std::vector<std::string> v(i-iter);
					copy(iter,i,v.begin());
					if ((int)v.size() != 0)
					{
						AddEdge(v);
					}
					iter = i;
				}
			}

			std::vector<std::string> v(i-iter);
			copy(iter,i,v.begin());
			if ((int)v.size() != 0)
			{
				AddEdge(v);
			}

			this->count = 0;
			std::map<Corpus::CValue,CNegRuleDecisionTreeNode*>::iterator k;
			for(k = edges->begin(); k != edges->end(); k++)
			{
				this->count += (*k).second->count;
				this->wCount += (*k).second->wCount;
			}
		}

		void CNegRuleDecisionTreeNode::AddEdge(std::vector<std::string>& edge)
		{
			std::string s = *edge.begin();
			edge.erase(edge.begin());

			std::vector<std::string> temp;
			Corpus::Parser::Tokenize(s,temp);
			Corpus::CValue argValue;

			argValue.first = Corpus::Parser::ParseDLONG(temp[2]);

			if( (int)edge.size() != 0 )
			{
				ParseBranches(edge);
			}
			else
			{
				edge.push_back(s.substr(s.find(':')+2));
			}
			(*edges)[argValue] = new CNegRuleDecisionTreeNode(edge);
		}
		//wypelnia wartosci prawdopodobienstw
		void CNegRuleDecisionTreeNode::UpdateProbabilities(unsigned int amount, double lambda, int possibleTags)
		{
			// Liczymy prawdopodobie�stwo w�z�a (wyg�adzone).
			probability = ((double)(this->count + lambda))/(((double)amount + (double)possibleTags*lambda));
			wProbability = lambda/(((double)amount + (double)possibleTags*lambda));

			// Liczymy prawdopodobie�stwa logarytmiczne (b��dne i normalne)
			lWrongProbability = log(wProbability);
			lProbability = log(probability);

			std::map<Corpus::CValue,CNegRuleDecisionTreeNode*>::iterator k;

			for(k = edges->begin(); k != edges->end(); k++)
			{
				(*k).second->UpdateProbabilities(amount,lambda,possibleTags);
			}
		}
		//pobieramy dane o lisciu
		void CNegRuleDecisionTreeNode::GetLeafInfo(std::string& leafInfo)
		{
			isNode = false;

			std::vector<std::string> v;
			Corpus::Parser::Tokenize(leafInfo,v);

			std::vector<std::string>::iterator iter = v.begin();

			//this->decision = (Corpus::DLONG)atol((*iter).c_str());
			this->decision = Corpus::Parser::ParseDLONG(*iter);
			iter++;
			std::string licznoscString = iter->substr(1,iter->size()-2);
			std::vector<std::string> licznosci;
			Corpus::Parser::Tokenize(licznoscString,licznosci,"/");
			this->count = atoi(licznosci.begin()->c_str());
			if((int)licznosci.size() > 1)
				this->wCount = atoi(licznosci.back().c_str());
			else
				this->wCount = 0;
		}
		// usuwa pojedynczy znak '|' i nast�puj�ce po nim bia�e spacje
		void CNegRuleDecisionTreeNode::ParseBranches(std::vector<std::string>& tree)
		{
			std::vector<std::string>::iterator iter;
			for(iter = tree.begin(); iter != tree.end(); iter++)
			{
				(*iter).erase(0,1);
				Corpus::Parser::TrimStart(*iter);
			}
		}

		void CNegRuleDecisionTreeNode::Serialize(std::ofstream& file)
		{
			file.write((char*)&isNode,sizeof(isNode));
			file.write((char*)&count,sizeof(count));
			file.write((char*)&lProbability,sizeof(lProbability));
			file.write((char*)&lWrongProbability,sizeof(lWrongProbability));
			file.write((char*)&probability,sizeof(probability));
			file.write((char*)&wProbability,sizeof(wProbability));
			file.write((char*)&attribute,sizeof(attribute));
			file.write((char*)&decision,sizeof(decision));

			int edgesSize = (int)edges->size();
			file.write((char*)&edgesSize,sizeof(edgesSize));

			for(std::map<Corpus::CValue,CNegRuleDecisionTreeNode*>::iterator i = edges->begin(); i != edges->end(); i++)
			{
				Corpus::DLONG pos = i->first.first;
				Corpus::DLONG gramCat = i->first.second;

				file.write((char*)&pos,sizeof(pos));
				file.write((char*)&gramCat,sizeof(gramCat));
				(*i).second->Serialize(file);
			}
		}

		STATIC CNegRuleDecisionTreeNode* CNegRuleDecisionTreeNode::Deserialize(std::ifstream& file)
		{
			CNegRuleDecisionTreeNode* newNode = new CNegRuleDecisionTreeNode();

			file.read((char*)&(newNode->isNode),sizeof(newNode->isNode));
			file.read((char*)&(newNode->count),sizeof(newNode->count));
			file.read((char*)&(newNode->lProbability),sizeof(newNode->lProbability));
			file.read((char*)&(newNode->lWrongProbability),sizeof(newNode->lWrongProbability));
			file.read((char*)&(newNode->probability),sizeof(newNode->probability));	
			file.read((char*)&(newNode->wProbability),sizeof(newNode->wProbability));	
			file.read((char*)&(newNode->attribute),sizeof(newNode->attribute));
			file.read((char*)&(newNode->decision),sizeof(newNode->decision));

			int edgesCount;
			file.read((char*)&edgesCount,sizeof(int));

			for(int i = 0; i < edgesCount; i++)
			{
				Corpus::DLONG pos;
				Corpus::DLONG gramCat;

				file.read((char*)&pos,sizeof(pos));
				file.read((char*)&gramCat,sizeof(gramCat));

				Corpus::CValue p(pos,gramCat);

				(*newNode->edges)[p] = CNegRuleDecisionTreeNode::Deserialize(file);
			}

			return newNode;
		}

		void CNegRuleDecisionTreeNode::UseNegRuleTree(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& sentence, 
			Corpus::CValue& mask)
		{
			if ((int)edges->size() == 0)
			{
				for(int i = 0; i < (int)sentence.GetCurrentToken()->GetLexCount(); i++)
				{
					Corpus::CLexem* l = sentence.GetCurrentToken()->GetLexem(i);
					if(this->decision == Corpus::negTree_False)
					{
						//je�li znaleziony lexem jest podmiotem szukania
						if((l->GetTag().FlexClass & mask.first) == mask.first)
							sentence.GetCurrentToken()->SetLexProbability(i,log(1-this->probability));
						//else
						//	sentence.GetCurrentToken()->SetLexProbability(i,log(1+this->probability));
					}
					//else
					//{
					//	//je�li znaleziony lexem nie jest podmiotem szukania
					//	if((l->GetTag().FlexClass & mask.first) != mask.first)
					//		sentence.GetCurrentToken()->SetLexProbability(i,log(1-sqrt(this->probability)));
					//	else
					//		sentence.GetCurrentToken()->SetLexProbability(i,log(1+sqrt(this->probability)));
					//}
				}
			}
			else
			{
				Corpus::CValue value;
				pattern.GetValueAt(this->attribute,sentence,this->attribute,value);

				if(value.first == 0)
				{
					Corpus::DLONG val = value.first;
					value.first = value.second;
					value.second = val;
				}

				CNegRuleDecisionTreeNode* edge = (*edges)[value];

				if (edge == 0)
				{
					CNegRuleDecisionTreeNode::statystyka ++;	
				}
				else
				{
					edge->UseNegRuleTree(pattern,sentence,mask);
				}
			}
		}

	}// namespace Classifiers
}//namespace TaKIPI
