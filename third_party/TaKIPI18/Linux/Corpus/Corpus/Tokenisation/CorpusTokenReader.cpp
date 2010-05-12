#include "../../corpuslib_headers.h"
#include "CorpusTokenReader.h"
#include "../Config.h"

namespace Corpus
{
	namespace Tokenisation
	{
		CCorpusTokenReader::CCorpusTokenReader(std::istream &co_inStream,
											   bool disambOnly, bool convertToCP)
			:CTokenReader(co_inStream), filterDisamb(disambOnly), convertOutput(convertToCP)
		{
		}
		
		CCorpusTokenReader::~CCorpusTokenReader()
		{
		}
		
		std::string CCorpusTokenReader::GetNextTokenText(std::istream& inStream)
		{
			std::string line;
			static const std::string empty;
			std::stringstream tokenStream;
			//char *buff = new char[512];

			do // skip all lines without tok or ns
			{ // what about beef?
				std::getline(inStream, line);
				if(!inStream)
				{
					return empty;
				}
			}
			while(line.find("<tok>") == line.npos
						   && line.find("<ns/>") == line.npos);

			do // gather all lines until </tok>
			{
				tokenStream << line << std::endl;
				std::getline(inStream, line);
				
				if(!inStream)
				{
					return empty;
				}
			}
			while(line.find("</tok>") == line.npos);

			tokenStream << line; // include </tok>
			return tokenStream.str();
		}
		
		CToken *CCorpusTokenReader::GetNext()
		{
			std::string tokenString = GetNextTokenText(co_input);
			if(tokenString.empty())
			{
				return NULL;
			}
			std::stringstream tokenStream;
			tokenStream << tokenString;

			CToken *token = new CToken(tokenStream);

			if(filterDisamb) // delete lexems without disamb marker
			{
				for(int i = 0; i < (int)token->GetLexCount(); i++)
				{
					if( token->GetLexem(i)->GetDisamb() != true)
					{
						token->RemoveLexem(i);
						i--;
					}
				}
				// leave only one disamb (actually delete the last one)
				// this is copied from the old code without change
				while(token->GetLexCount() > 1)
				{
					token->RemoveLexem((int)token->GetLexCount()-1);
				}
			}
			
			if(convertOutput)
			{
				token->ConvertEncoding(Corpus::CConfig::inputEncoding, Corpus::Parser::cp1250);
			}
			return token;
		}
		
	} // namespace Tokenisation
} // namespace Corpus

