#include "../../corpuslib_headers.h"
#include "WindowContext.h"

namespace Corpus
{
	namespace Context
	{

		WindowContext::WindowContext(Corpus::Documents::Document* doc, int left, int right)
			:Context(doc)
		{
			before = left;
			after = right;
			currentToken = NULL;
		}

		WindowContext::~WindowContext()
		{
			while(tokensAfter.size())
			{
				CToken* temp = tokensAfter[0];
				tokensAfter.erase(tokensAfter.begin());
				delete temp;
			}

			while(tokensBefore.size())
			{
				CToken* temp = tokensBefore[0];
				tokensBefore.erase(tokensBefore.begin());
				delete temp;
			}

			if(currentToken != NULL)
				delete currentToken;
		}

		bool WindowContext::FirstPosition()
		{
			document->Close();
			document->Open();

			currentToken = this->document->NextToken();

			if(currentToken == NULL)
				return false;

			for(int i = 0; i < after ; i++)
			{
				CToken* w = document->NextToken();
				if(w == NULL)
					break;
				else
					tokensAfter.push_back(w);
			}

			return true;
		}

		bool WindowContext::NextPosition()
		{
			if(tokensAfter.empty())
				return false;

			CToken* w = this->document->NextToken();
			if(w != NULL)
				tokensAfter.push_back(w);

			tokensBefore.push_back(currentToken);
			currentToken = tokensAfter[0];

			tokensAfter.erase(tokensAfter.begin());

			if((int)tokensBefore.size() > before)
			{
				CToken* temp = tokensBefore[0];
				tokensBefore.erase(tokensBefore.begin());
				delete temp;
			}

			return true;
		}

	// 	bool WindowContext::PierwszaPozycjaFiltrowana()
	// 	{
	// 		if(!PierwszaPozycja())
	// 			return false;
	// 		if(obecnyWyraz_->CzyFiltrowany())
	// 			return true;
	//
	// 		NastepnaPozycjaFiltrowana();
	//
	// 		return true;
	// 	}
	//
	// 	bool WindowContext::NastepnaPozycjaFiltrowana()
	// 	{
	// 		do
	// 		{
	// 			if(!NastepnaPozycja())
	// 				return false;
	// 		}
	// 		while(!obecnyWyraz_->CzyFiltrowany());
	//
	// 		return true;
	// 	}

		CToken* WindowContext::operator[](int n)
		{
			if(n == 0)
				return currentToken;
			else if(n> 0 && n <= (int)tokensAfter.size())
				return(tokensAfter[n-1]);
			else if(n < 0 && -n <= (int)tokensBefore.size())
				return(tokensBefore[(int)tokensBefore.size()+n]);
			else
				return NULL;
		}
	}
}
