#include "../../takipi_headers.h"
#include "LMTreeNode.h"
#include "DecisionTreeNode.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		CLMTreeNode::CLMTreeNode(std::vector<std::string>& treedata, CLogisticModel * models)
		{
			edges = new std::map<Corpus::CValue,CLMTreeNode*> ();
			while( !(treedata.begin()->length()))
			{
				treedata.erase(treedata.begin());
			}

			std::string s = *treedata.begin();

			if (s.find("=") == s.npos)
			{
				GetLeafInfo(s,models);
				return;
			}

			std::vector<std::string> temp;
			Corpus::Parser::Tokenize(s,temp);

			this->attribute = atoi((*temp.begin()).c_str());

			BuildEdges(treedata,models);
		}

		CLMTreeNode::~CLMTreeNode(void)
		{
			if(model)
				delete model;
			if(edges)
			{
			CLMTreeNode* tempEdge;
			for(std::map<Corpus::CValue,CLMTreeNode*>::iterator i = edges->begin(); i != edges->end(); i++)
			{
				tempEdge = i->second;
				delete tempEdge;
			}
			delete edges;
			}
		}

		void CLMTreeNode::GetLeafInfo(std::string& leafInfo, CLogisticModel *models)
		{
			std::vector<std::string> tokens;
			leafInfo.erase(0,leafInfo.find("LM_"));
			Corpus::Parser::Tokenize(leafInfo,tokens,"_:");
			this->model = models + (atoi(tokens[1].c_str()) - 1);
			//tutaj mozna by jeszcze wziac takie dane jak nlogitboost/totallogitbosst (count)
		}

		void CLMTreeNode::AddEdge(std::vector<std::string> & edge, CLogisticModel *models)
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
			(*edges)[argValue] = new CLMTreeNode(edge, models);
		}

		// usuwa pojedynczy znak '|' i nastêpuj¹ce po nim bia³e spacje
		void CLMTreeNode::ParseBranches(std::vector<std::string>& tree)
		{
			std::vector<std::string>::iterator iter;
			for(iter = tree.begin(); iter != tree.end(); iter++)
			{
				iter->erase(0,1);
				Corpus::Parser::TrimStart(*iter);
			}
		}

		void CLMTreeNode::BuildEdges(std::vector<std::string>& tree, CLogisticModel *models)
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
						AddEdge(v,models);
					}
					iter = i;
				}
			}

			std::vector<std::string> v(i-iter);
			copy(iter,i,v.begin());
			if ((int)v.size() != 0)
			{
				AddEdge(v,models);
			}

			//this->count = 0;
			//map<CValue,CLMTreeNode*>::iterator k;
			//for(k = edges.begin(); k != edges.end(); k++)
			//	this->count += (*k).second->count;
		}

		Corpus::CToken* CLMTreeNode::Unify(JOSKIPI::Patterns::CPattern& pattern, Corpus::CSentence& srcSen, TaKIPI::CLayers& layers)
		{
			if(!edges->size())
			{
				Corpus::CValue mask,temp;
				layers.GetLayerMask(mask);

				std::map<Corpus::CValue,double> logProbs = this->model->GetLogs(pattern,srcSen);

				for(int i = 0; i < (int)srcSen.GetCurrentToken()->GetLexCount(); i++)
				{
					(*(srcSen.GetCurrentToken()->GetLexem(i))).GetMaskedValue(mask,temp);
					srcSen.GetCurrentToken()->SetLexProbability(i,logProbs[temp]);
				}
				return 0;
			}

			Corpus::CValue value;
			pattern.GetValueAt(this->attribute,srcSen,this->attribute,value);

			if(value.first == 0)
			{
				Corpus::DLONG val = value.first;
				value.first = value.second;
				value.second = val;
			}

			CLMTreeNode* son = (*edges)[value];

			if (son == NULL)
			{
				CDecisionTreeNode::statystyka ++;//ew. mozna rozbic na zgubienie sie drzew LMT osobno niz c45
			}
			else
			{
				son->Unify(pattern,srcSen,layers);
			}

			return 0;
		}
	}// namespace Classifiers
}//namespace TaKIPI

