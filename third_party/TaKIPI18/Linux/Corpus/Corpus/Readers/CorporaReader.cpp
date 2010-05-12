#include "../../corpuslib_headers.h"
#include "CorporaReader.h"

namespace Corpus
{
	namespace Readers
	{
		CCorporaReader::CCorporaReader(std::istream& stream,  bool dissambOnly )
			:CReader(stream), m_bDissambOnly(dissambOnly)
		{
		}

		CCorporaReader::~CCorporaReader(void)
		{
		}

		CToken* CCorporaReader::GetToken()
		{
			std::string tokenString = GetNextTokenText(stream); // z CReadera
			if(tokenString.empty())
				return 0;

			std::stringstream tokenStream;
			tokenStream << tokenString;

			CToken *token = new CToken(tokenStream);

			if(m_bDissambOnly) // usuwamy tokeny bez disamba
			{
				for(int i = 0; i < (int)token->GetLexCount(); i++)
				{
					if( token->GetLexem(i)->GetDisamb() != true)
					{
						token->RemoveLexem(i);
						i--;
					}
				}
				//zostawic tylko 1 disamba
				while(token->GetLexCount() > 1)
				{
					token->RemoveLexem((int)token->GetLexCount()-1);
				}

			}

			return token;
		}
	}

}
