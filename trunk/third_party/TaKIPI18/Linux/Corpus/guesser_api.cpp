#include "corpuslib_headers.h"
#include "guesser_api.h"
#include "Corpus/GuessingSystem/FormsIdentifier.h"
#include "Corpus/Parser/Parser.h"
#include "Corpus/Token.h"
#include "Corpus/PlainWriter.h"
#include "Corpus/Config.h"
#include "Corpus/ExtSentencer/LowLevelSentencer.h"
#include "Corpus/ResourceManager.h"

CORPUS_API void SetCorpusEncoding(int encoding)
{
	Corpus::CConfig::inputEncoding = (Corpus::Parser::Encoding)encoding;
}

CORPUS_API void SetCorpusDataDir(const char *dir)
{
	if(dir != NULL && dir[0] != 0)
	{
		Corpus::CConfig::SetPathsToDirectory(dir);
	}
}

CORPUS_API const char *GuessForm(const char *form)
{
	Corpus::GuessingSystem::FormsIdentifier *fi = Corpus::CResourceManager::GetInstance()->GetFormsIdentifier();
	const char *output = NULL;
	std::vector<Corpus::CToken *> tokens;
	fi->Identify(form, tokens);
	output = Corpus::PlainWriter::GeneratePlainTagString(tokens);
	fi->FreeTokenVector(tokens);
	return output;
}

CORPUS_API const char *GuessFormXml(const char *form)
{
	Corpus::GuessingSystem::FormsIdentifier *fi = Corpus::CResourceManager::GetInstance()->GetFormsIdentifier();
	const char *co_output = NULL;
	std::vector<Corpus::CToken *> tokens;
	fi->Identify(form, tokens);
	co_output = Corpus::PlainWriter::GenerateXmlTagString(tokens);
	fi->FreeTokenVector(tokens);
	return co_output;
}

CORPUS_API const char *TokeniseInputXml(const char *co_input)
{
	const char *co_output = NULL;
	std::stringstream co_stream(co_input);
	Corpus::Tokenisation::CTokeniser tokr(co_stream, false, true);
	co_output = Corpus::PlainWriter::GenerateXmlTagString(tokr);
	return co_output;
}

CORPUS_API int SentencerFeed(const char *orth, const bool wasNS, const bool isSym)
{
	Corpus::ExtSentencer::CLowLevelSentencer *sentr =
		Corpus::CResourceManager::GetInstance()->GetDefaultSentencer();
	return sentr->Feed(orth, wasNS, isSym);
}

CORPUS_API int SentencerFed()
{
	Corpus::ExtSentencer::CLowLevelSentencer *sentr =
		Corpus::CResourceManager::GetInstance()->GetDefaultSentencer();
	return sentr->Fed();
}

CORPUS_API void SentencerReset()
{
	Corpus::ExtSentencer::CLowLevelSentencer *sentr =
		Corpus::CResourceManager::GetInstance()->GetDefaultSentencer();
	sentr->Reset();
}
