#include "../corpuslib_headers.h"
#include "Transducer.h"

namespace Corpus
{
	std::map<int,CToken*>* CTransducer::tokens = NULL;
	std::map<std::string,int>* CTransducer::tokenLabels = NULL;
	size_t CTransducer::transducerCount = 0;

	CTransducer::CTransducer()
	{
		transducerCount++;
		root = new CTransducerNode();
		actualState_ = root;
		if(tokens == NULL)
			tokens = new std::map<int,CToken*>();
		if(tokenLabels == NULL)
			tokenLabels = new	std::map<std::string,int> ();
	}

	CTransducer* CTransducer::ParseFromFile(std::string fileName)
	{
		std::ifstream file;
		file.open(fileName.c_str());
		char buff[512];

		if(file.is_open())
		{
			CTransducer* t = new CTransducer();

			
			std::string linia;

			std::vector<std::string> rule;
			do
			{
				file.getline(buff,512);
				linia = buff;
				if(linia.empty())
					continue;
				rule.push_back(linia);
				if(linia[linia.size()-1] == ']')
					ParseRule(rule,t);
			}
			while(!file.eof());

			return t;
		}
		else
		{
			return 0;
		}
	}

	void CTransducer::ParseRule(std::vector<std::string>& rule, CTransducer *t)
	{
		std::string prerequisite = rule[0].substr(1,rule[0].size()-4);
		std::stringstream tokenStream;
		for(int i = 1; i < (int)rule.size()-1; i++)
		{
			tokenStream << rule[i] << std::endl;
		}

		Corpus::Readers::CCorporaReader reader(tokenStream);

		std::vector<int> tokensInts;
		CToken* token = reader.GetToken();
		while(token)
		{
			if(tokenLabels->find(token->GetOrth()) == tokenLabels->end())
			{
				(*tokens)[(int)tokens->size()] = token;
				(*tokenLabels)[token->GetOrth()] = (int)tokens->size() - 1;
				tokensInts.push_back((int)tokens->size() - 1);
			}
			else
			{
				tokensInts.push_back((*tokenLabels)[token->GetOrth()]);
			}
			token = reader.GetToken();
		}

		t->AddNewForm(prerequisite,tokensInts);

		rule.clear();
	}

	CTransducer::~CTransducer(void)
	{
		CTransducer::transducerCount--;

		if(root)
			delete root;

		if(CTransducer::transducerCount == 0)// last transducer
		{
			if(tokens)
			{
				for(std::map<int,CToken*>::iterator i = tokens->begin(); i != tokens->end(); i++)
				{
					CToken *t = i->second;
					t->RefCount--;
					if(t->RefCount <= 0)
						delete t;
				}
				delete tokens;
				tokens = NULL;
			}
			if(tokenLabels)
			{
				delete tokenLabels;
				tokenLabels = NULL;
			}
		}
	}

	void CTransducer::AddNewForm(const std::string& form, const std::vector<int>& outForm)
	{
		root->AddNewForm(form.c_str(),outForm);
	}

	bool CTransducer::CheckForm(const std::string& form)
	{
		return root->CheckForm(form.c_str());
	}

	std::vector<CToken*>* CTransducer::Transduce(const std::string& form)
	{
		std::vector<int>& tokensInts = root->Transduce(form.c_str());
		std::vector<CToken*>* tokensOut = new std::vector<CToken*>();
		for(int i = 0; i < (int)tokensInts.size(); i++)
			tokensOut->push_back((*tokens)[tokensInts[i]]);

		return tokensOut;
	}

	bool CTransducer::PerformStep(char character)
	{
		actualState_ = actualState_->PerformStep(character);
		return actualState_ != 0;
	}

	void CTransducer::GoToStart()
	{
		actualState_ = root;
	}

	bool CTransducer::IsValidState()
	{
		return actualState_->IsValid();
	}

	void CTransducer::Serialize(std::string fileName)
	{
		throw std::string("CTransducer::Serialize not implemented!");
		//ofstream file;
		//file.open(fileName.c_str(),ios::binary);

		//// Zapisanie tokens
		//// iloœæ
		//file.write((char*)&tokens.size(),sizeof(int));
		//map<int,CToken*>::iterator i = tokens.begin();
		//// dla ka¿dego
		//for(;i != tokens.end(); i++)
		//{
		//	//identyfikator tokenu
		//	file.write((char*)&i->first,sizeof(int));
		//	i->second->Serialize(file);
		//}

		////Zapisanie tokensLabels
		//// iloœæ
		//file.write((char*)&tokenLabels.size(),sizeof(int));
		//map<string,int>::iterator j = tokenLabels.begin();
		//// dla ka¿dego
		//for(;j != tokenLabels.end(); j++)
		//{
		//	//identyfikator tokenu
		//	file.write((char*)&i->first,sizeof(int));
		//	i->second->Serialize(file);
		//}
		////Zapisanie root;



		//file.flush();
		//file.close();
	}

}

