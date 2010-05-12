#include "../../corpuslib_headers.h"
#include "DocumentContext.h"

namespace Corpus
{
	namespace Context
	{
		DocumentContext::DocumentContext(Corpus::Documents::Document* document) :Context(document)
		{
			document->GotoStart();
			CToken* t = document->NextToken();
			do
			{
				tokens.push_back(t);
				t = document->NextToken();
			}
			while(t != NULL);
		}

		DocumentContext::~DocumentContext(void)
		{
			for(int i = 0; i < (int)tokens.size(); i++)
			{
				CToken* w = tokens[i];
				delete w;
			}
		}

		CToken* DocumentContext::operator[](int n)
		{
			n = n + position;

			if(n >= 0 && n < (int)tokens.size())
				return(tokens[n]);
			else
				return NULL;
		}
	}
}
