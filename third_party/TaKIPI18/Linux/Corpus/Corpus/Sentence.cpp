#include "../corpuslib_headers.h"
#include "Sentence.h"

namespace Corpus
{

	CSentence::CSentence()
	{
		m_bIOwnTokens = true;
		blankToken = CToken::CreateBlank();
		chosenToken = CPosition::nowhere;
	}
	CSentence::CSentence(std::vector<CToken*> &sentence, bool removeTokensInDtor)
	{
		m_bIOwnTokens = removeTokensInDtor;
		blankToken = CToken::CreateBlank();

		for(int i = 0; i < (int)sentence.size(); i++)
		{
			this->sentence.push_back(sentence[i]);
		}
		chosenToken = CPosition::nowhere;
	}
	CSentence::CSentence(std::deque<CToken*> &sentence, bool removeTokensInDtor)
	{
		m_bIOwnTokens = removeTokensInDtor;
		blankToken = CToken::CreateBlank();
		chosenToken = CPosition::nowhere;

		for(std::deque<CToken*>::iterator iter = sentence.begin(); iter != sentence.end(); iter ++)
			this->sentence.push_back(*iter);
	}

	CSentence::~CSentence(void)
	{
		if(m_bIOwnTokens)
		{
			for(std::vector<CToken*>::iterator i = sentence.begin(); i != sentence.end(); i++)
			{
				CToken* token = *i;
				if(token->RefCount == 1)
				{
					delete token;
					token = NULL;
				}
				else
					token->RefCount--;
			}
		}

		if(blankToken && (--blankToken->RefCount) == 0)
		{
			delete blankToken;
			blankToken = NULL;
		}
	}

	void CSentence::AddToken(CToken *token)
	{
		sentence.push_back(token);
	}

	void CSentence::Goto(int position)
	{
		if(position < 0 || position >= (int)sentence.size())
			chosenToken = CPosition::nowhere;
		else
			chosenToken = position;
	}

	int CSentence::GetPosition()
	{
		return chosenToken;
	}

	// Ustawia wskaznik obecnego tokena na pierwszy element
	bool CSentence::FirstPosition()
	{
		chosenToken = -1;

		return NextPosition();
	}

	// Ustawia wskaznik obecnego tokena na nast�pny element
	bool CSentence::NextPosition()
	{
		chosenToken++;

		if (chosenToken < Size() && chosenToken >= 0)
			return true;
		else if (chosenToken < 0 && Size() > 0)
		{
			chosenToken = 0;
			return true;
		}
		else
		{
			chosenToken = CPosition::nowhere;
			return false;
		}
	}

	/**
	*  Creates Shallow copy.
	*	
	*	\todo Check if this works...
	*/
	CSentence* CSentence::CreateShallowCopy()
	{
		std::vector<CToken*> tokens;

		for(int i = 0; i < this->Size(); i++)
		{
			CToken* blank = CToken::CreateBlank(this->sentence[i]->GetOrth());
			tokens.push_back(blank);
		}

		CSentence* s = new CSentence(tokens);
		return s;
	}

	// Creates copy of the sentence (the same lexems).
	CSentence* CSentence::CreateCopy()
	{
		CSentence* s = new CSentence();

		for(int i = 0; i < this->Size(); i++)
		{
			CToken* t = sentence[i]->CreateCopy();
			for(int j = 0; j < (int)t->GetLexCount(); j++)
				t->GetLexem(j)->probability = 0;
			s->AddToken(t);
		}

		return s;
	}

	CSentence* CSentence::CreateHardCopy()
	{
		CSentence* s = new CSentence();

		for(int i = 0; i < this->Size(); i++)
		{
			CToken* t = sentence[i]->CreateHardCopy();
			//for(int j = 0; j < (int)t->GetLexCount(); j++)
			//	t->GetLexem(j)->probability = 0;
			s->AddToken(t);
		}
		s->chosenToken = this->chosenToken;
		return s;
	}
	// Creates copy of the sentence with only disamb lexems.
	CSentence* CSentence::CreateDisambCopy()
	{
		std::vector<CToken*> tokens;

		for(int i = 0; i < this->Size(); i++)
		{
			CToken* blank = CToken::CreateBlank(this->sentence[i]->GetOrth());
			blank->ClearLexems();

			for(int j = 0; j < (int)sentence[i]->GetLexCount(); j++)
			{
				if(sentence[i]->GetLexem(j)->GetDisamb())
				{
					sentence[i]->GetLexem(j)->RefCount++;
					blank->AddLexem(sentence[i]->GetLexem(j));
				}
			}

			tokens.push_back(blank);
		}

		CSentence* s = new CSentence(tokens);
		s->chosenToken = this->chosenToken;
		return s;
	}
	void CSentence::Replace(CToken* token, int position)
	{
		CToken* t = sentence[position];
		if(m_bIOwnTokens)
		{
			if(t->RefCount == 1)
				delete t;
			else
				t->RefCount--;
		}

		sentence[position] = token;
	}

	std::string CSentence::ToXmlString(bool writeProbs)
	{
		std::stringstream ss;
		for(size_t tok_num = 0; tok_num < sentence.size(); tok_num++)
			ss << sentence[tok_num]->ToXmlString(writeProbs) << std::endl;
		return ss.str();
	}

	void CSentence::ConvertEncoding(Parser::Encoding fromEnc, Parser::Encoding toEnc)
	{
		for(size_t tok_num = 0; tok_num < sentence.size(); tok_num++)
		{
			sentence[tok_num]->ConvertEncoding(fromEnc, toEnc);
		}
	}

	void CSentence::EliminateSubstGerIgn()
	{
		for(int toknum = 0; toknum < this->Size(); toknum++)
		{
			sentence[toknum]->PreprocessSubstGerIgn();
		}
	}

	CToken* CSentence::GetToken(int position)
	{
		if (position < 0 || position >= Size())
			return blankToken;
		return sentence[position];
	}

	std::string CSentence::ToOrthString()
	{
		std::stringstream out;

		for(size_t tok_num = 0; tok_num < sentence.size(); tok_num++)
			out << sentence[tok_num]->GetOrth() << " ";

		return out.str();
	}
	
	void CSentence::ClearDisambsAndProbs()
	{
		for(size_t tok_num = 0; tok_num < sentence.size(); tok_num++)
		{
			sentence[tok_num]->ClearDisambsAndProbs();
		}
	}

	void CSentence::EraseTokens(const std::set<size_t> &indices)
	{
		std::vector<CToken*> new_sentence;

		for(size_t tok_num = 0; tok_num < sentence.size(); tok_num++)
		{
			CToken *token = sentence[tok_num];

			if( indices.count(tok_num) == 0)
				new_sentence.push_back(token);
			else
			{
				if(m_bIOwnTokens)
				{
					if(token->RefCount == 1)
					{
						delete token;
						token = NULL;
					}
					else
						token->RefCount--;
				}
			}
		}

		sentence = new_sentence;
	}
}
