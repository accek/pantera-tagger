#include "../corpuslib_headers.h"
#include "TransducerNode.h"

namespace Corpus
{
	CTransducerNode::CTransducerNode(void)
	{
		isRecursive_ = false;
		nodes = new std::map<char,CTransducerNode*>();
	}

	CTransducerNode::~CTransducerNode(void)
	{
		for(std::map<char,CTransducerNode*>::iterator iter = nodes->begin();
			iter != nodes->end(); iter++)
		{
			CTransducerNode* node = iter->second;
			if(!isRecursive_)
				delete node;
		}

		if(nodes)
		{
			delete nodes;
			nodes = NULL;
		}
	}

	void CTransducerNode::AddNewForm(const char *form, const std::vector<int>& finalFormCode)
	{
		if(form == 0)
			return;

		if(strlen(form) == 0)
		{
			finalForm = finalFormCode;
			return;
		}

		if(form[1] == '|')
		{
			AddUnionForm(form,finalFormCode);
		}
		else if(form[0] == '(')
		{
			AddOptionalityForm(form,finalFormCode);
		}
		else if(form[1] == '+')
		{
			AddIterationForm(form,finalFormCode);
		}
		else if(form[1] == '*')
		{
			AddStarForm(form,finalFormCode);
		}
		else
		{
			CTransducerNode *childNode;

			if(nodes->find(form[0]) == nodes->end())
			{
				childNode = new CTransducerNode();
				(*nodes)[form[0]] = childNode;
			}
			else
				childNode = (*nodes)[form[0]];

			childNode->AddNewForm(form+1, finalFormCode);
		}
	}

	void CTransducerNode::AddUnionForm(const char* form, const std::vector<int>& finalFormCode)
	{
		CTransducerNode *childNode;

		if(nodes->find(form[0]) == nodes->end())
		{
			childNode = new CTransducerNode();
			(*nodes)[form[0]] = childNode;
		}
		else
			childNode = (*nodes)[form[0]];

		childNode->AddNewForm(form+3, finalFormCode);

		if((*nodes).find(form[2]) == nodes->end())
		{
			childNode = new CTransducerNode();
			(*nodes)[form[2]] = childNode;
		}
		else
			childNode = (*nodes)[form[2]];

		childNode->AddNewForm(form+3, finalFormCode);
	}

	void CTransducerNode::AddOptionalityForm(const char* form, const std::vector<int>& finalFormCode)
	{
		CTransducerNode *childNode;

		if(nodes->find(form[1]) == nodes->end())
		{
			childNode = new CTransducerNode();
			(*nodes)[form[1]] = childNode;
		}
		else
			childNode = (*nodes)[form[1]];

		childNode->AddNewForm(form+3, finalFormCode);

		this->AddNewForm(form+3, finalFormCode);
	}

	void CTransducerNode::AddIterationForm(const char* form, const std::vector<int>& finalFormCode)
	{
		CTransducerNode *childNode;

		if(nodes->find(form[0]) == nodes->end())
		{
			childNode = new CTransducerNode();
			(*nodes)[form[0]] = childNode;
		}
		else
			childNode = (*nodes)[form[0]];

		childNode->AddStarForm(form, finalFormCode);
	}

	void CTransducerNode::AddStarForm(const char* form, const std::vector<int>& finalFormCode)
	{
		isRecursive_ = true;
		(*nodes)[form[0]] = this;

		this->AddNewForm(form+2, finalFormCode);
	}

	bool CTransducerNode::CheckForm(const char* form)
	{
		if(strlen(form) == 0)
			return finalForm.empty();
		else if(nodes->find(form[0]) == nodes->end())
			return false;
		else
		{
			return ((*nodes)[form[0]])->CheckForm(form+1);
		}
	}

	std::vector<int>& CTransducerNode::Transduce(const char* form)
	{
		if(strlen(form) == 0)
			return finalForm;
		else if(nodes->find(form[0]) == nodes->end())
			return finalForm;
		else
		{
			return ((*nodes)[form[0]])->Transduce(form+1);
		}
	}

	CTransducerNode* CTransducerNode::PerformStep(char character)
	{
		if(nodes->find(character) == nodes->end())
			return 0;
		else
			return (*nodes)[character];
	}
}
