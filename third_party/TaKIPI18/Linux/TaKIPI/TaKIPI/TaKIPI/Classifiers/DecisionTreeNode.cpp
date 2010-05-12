#include "../../takipi_headers.h"
#include "DecisionTreeNode.h"


namespace TaKIPI
{
	namespace Classifiers
	{
		Corpus::DLONG CDecisionTreeNode::statystyka = 0;

		CDecisionTreeNode::CDecisionTreeNode()
		{
			edges = new std::map<Corpus::CValue,CDecisionTreeNode*>();
		}
		//tworzy drzewo z podanego wektora string'ow
		CDecisionTreeNode::CDecisionTreeNode(std::vector<std::string>& tree)
		{
			edges = new std::map<Corpus::CValue,CDecisionTreeNode*>();
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

#ifdef OCR
		void CDecisionTreeNode::Count(double& probsSum, int& nodesCount)
		{
			for(map<CValue,CDecisionTreeNode*>::iterator i = edges.begin(); i != edges.end(); i++)
			{
				i->second->Count(probsSum,nodesCount);
			}

			if((int)edges.size() == 0)
			{
				//probsSum += this->probability;
				//++nodesCount;
				if(probsSum > this->lProbability)
					probsSum = this->lProbability;
				nodesCount = 1;
			}
		}
#endif

		CDecisionTreeNode::~CDecisionTreeNode(void)
		{
			if(edges)
			{
				CDecisionTreeNode* tempEdge;
				for(std::map<Corpus::CValue,CDecisionTreeNode*>::iterator i = edges->begin(); i != edges->end(); i++)
				{
					tempEdge = i->second;
					delete tempEdge;
				}
				delete edges;
			}
		}

		void CDecisionTreeNode::BuildEdges(std::vector<std::string>& tree)
		{
			std::vector<std::string>::iterator iter = tree.begin();
			std::vector<std::string>::iterator i;

			//dla kazdej krawedzi budujemy na koncu niej wezel
			for(i = tree.begin(); i != tree.end(); i++)
			{
				boost::algorithm::trim_left(*i);
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
			std::map<Corpus::CValue,CDecisionTreeNode*>::iterator k;
			for(k = edges->begin(); k != edges->end(); k++)
			{
				this->count += (*k).second->count;
				this->wCount += (*k).second->wCount;
			}
		}

		void CDecisionTreeNode::AddEdge(std::vector<std::string>& edge)
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
			(*edges)[argValue] = new CDecisionTreeNode(edge);
		}

		//wypelnia wartosci prawdopodobienstw
		void CDecisionTreeNode::UpdateProbabilities(unsigned int amount, 
			double lambda, int possibleTags)
		{
			// Liczymy prawdopodobieñstwo wêz³a (wyg³adzone).
			if(this->edges->size() == 0 && this->count == 0)
				this->count = 1;
			probability = ((double)(this->count + lambda))/(((double)amount + (double)possibleTags*lambda));
			wProbability = lambda/(((double)amount + (double)possibleTags*lambda));

			// Liczymy prawdopodobieñstwa logarytmiczne (b³êdne i normalne)
			lWrongProbability = log(wProbability);
			lProbability = log(probability);

			std::map<Corpus::CValue,CDecisionTreeNode*>::iterator k;

			for(k = edges->begin(); k != edges->end(); k++)
			{
				(*k).second->UpdateProbabilities(amount,lambda,possibleTags);
			}
		}

		//pobieramy dane o lisciu
		void CDecisionTreeNode::GetLeafInfo(std::string& leafInfo)
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

		// usuwa pojedynczy znak '|' i nastêpuj¹ce po nim bia³e spacje
		void CDecisionTreeNode::ParseBranches(std::vector<std::string>& tree)
		{
			std::vector<std::string>::iterator iter;
			for(iter = tree.begin(); iter != tree.end(); iter++)
			{
				(*iter).erase(0,1);
				Corpus::Parser::TrimStart(*iter);
			}
		}

		// Ujednoznacznia dany token centralny w oknie
		Corpus::CToken* CDecisionTreeNode::Unify(JOSKIPI::Patterns::CPattern& pattern, 	Corpus::CSentence& srcSen, 
			TaKIPI::CLayers& layers)
		{
			Corpus::CToken* token = NULL;

			if ((int)edges->size() == 0)
			{
				token = srcSen.GetCurrentToken()->CreateHardCopy();

				Corpus::CValue mask,temp;
				layers.GetLayerMask(mask);

				for(int i = 0; i < (int)token->GetLexCount(); i++)
				{
					(*token->GetLexem(i)).GetMaskedValue(mask,temp);
					if(mask.second == 0)
					{
						if(temp.first == this->decision)
							token->GetLexem(i)->probability = lProbability;
						else
							token->GetLexem(i)->probability = lWrongProbability;
					}
					else
					{
						if(temp.second == this->decision)
							token->GetLexem(i)->probability = lProbability;
						else
							token->GetLexem(i)->probability = lWrongProbability;
					}
				}
				return token;
			}

			Corpus::CValue value;
			pattern.GetValueAt(this->attribute,srcSen,this->attribute,value);

			if(value.first == 0)
			{
				Corpus::DLONG val = value.first;
				value.first = value.second;
				value.second = val;
			}

			CDecisionTreeNode* edge = (*edges)[value];

			if (edge == NULL)
			{
				CDecisionTreeNode::statystyka ++;	
			}
			else
			{
				token = edge->Unify(pattern,srcSen,layers);	
			}

			return token;
		}

		void CDecisionTreeNode::Serialize(std::ofstream& file)
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
			file.write((char*)&edgesSize,sizeof(int));

			int x = 0;
			for(std::map<Corpus::CValue,CDecisionTreeNode*>::iterator i = edges->begin(); i != edges->end(); i++)
			{
				Corpus::DLONG pos = i->first.first;
				Corpus::DLONG gramCat = i->first.second;

				//std::cout << pos << " " << gramCat<< std::endl;

				file.write((char*)&pos,sizeof(Corpus::TFlexClass));
				file.write((char*)&gramCat,sizeof(Corpus::DLONG));

				(*i).second->Serialize(file);
				x++;
			}
		}

		STATIC CDecisionTreeNode* CDecisionTreeNode::Deserialize(std::ifstream& file)
		{
			
			CDecisionTreeNode* newNode = new CDecisionTreeNode();

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
				Corpus::TFlexClass pos;
				Corpus::DLONG gramCat;

				file.read((char*)&pos,sizeof(Corpus::TFlexClass));
				file.read((char*)&gramCat,sizeof(Corpus::DLONG));

				Corpus::CValue p(pos,gramCat);

				//std::cout << p.first << " " << p.second << std::endl;

				/*if(newNode->edges->find(p) != newNode->edges->end())
					throw std::exception("\nKrakrakra\n");*/

				(* newNode->edges)[p] = CDecisionTreeNode::Deserialize(file);
				
			}

			return newNode;
		}

	}// namespace Classifiers
}//namespace TaKIPI


