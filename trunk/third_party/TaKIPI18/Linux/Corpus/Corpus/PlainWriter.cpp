#include "../corpuslib_headers.h"
#include "PlainWriter.h"

namespace Corpus
{
	static std::string bufferedOutput;

	const char *PlainWriter::GeneratePlainTagString(std::vector<Corpus::CToken *> &tokens)
	{
		std::ostringstream out;

		for(size_t tnum = 0; tnum < tokens.size(); tnum++)
		{
			CToken *tok = tokens[tnum];
			out << tok->GetOrth() << "\t";
			for(size_t lnum = 0; lnum < tok->GetLexCount(); lnum++)
			{
				CLexem *lex = tok->GetLexem((int)lnum);
				out << lex->GetBase() << " " << lex->GetTag().ToString();
				if(lnum + 1 < tok->GetLexCount())
					out << "\t";
			}
			out << std::endl;
		}

		bufferedOutput = out.str();
		return bufferedOutput.c_str();
	}

	const char *PlainWriter::GenerateXmlTagString(std::vector<Corpus::CToken *> &tokens)
	{
		std::ostringstream out;

		for(size_t tnum = 0; tnum < tokens.size(); tnum++)
		{
			CToken *tok = tokens[tnum];
			out << tok->ToXmlString();
		}

		bufferedOutput = out.str();
		return bufferedOutput.c_str();
	}

	const char *PlainWriter::GenerateXmlTagString(Tokenisation::CTokeniser &tokr)
	{
		std::ostringstream out;
		CToken *tok;
		while((tok = tokr.GetNext()) != NULL)
		{
			out << tok->ToXmlString();
			Tokenisation::CTokenReader::Free(tok);
		}

		bufferedOutput = out.str();
		return bufferedOutput.c_str();
	}
}
