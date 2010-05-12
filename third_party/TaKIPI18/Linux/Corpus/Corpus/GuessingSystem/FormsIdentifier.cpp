#include "../../corpuslib_headers.h"

#include "FormsIdentifier.h"
#include "../Morfeusz/morfeusz.h"
#include "../Parser/Parser.h"

#include "../Config.h"
#include "BaseEnding/BaseEndingDict.h"
#include "BaseEnding/Prefixes.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		FormsIdentifier::FormsIdentifier(Parser::Encoding inputEncoding, Parser::Encoding outputEncoding, int usage)
			:guesserUsage(usage),inputEncoding(inputEncoding),outputEncoding(outputEncoding)
		{
			this->guesser = NULL;
			Morfeusz::SetEncoding(MORFEUSZ_CP1250);

			if(guesserUsage != guessNone)
			{
				this->LoadGuesser(CConfig::GetEndingTreePath().c_str(),
								  CConfig::GetEndingTagSetPath().c_str());
				BaseEnding::BaseEndingDict::Deserialize();
				BaseEnding::Prefixes::Deserialize();
			}
		}

		FormsIdentifier::~FormsIdentifier(void)
		{
			if(this->guesser != NULL)
				delete this->guesser;

		}

		void FormsIdentifier::LoadGuesser(std::string endingTreeFile, std::string endingTagSetsFile)
		{
			this->guesser = new GuessingSystem::Guesser(endingTreeFile.c_str(), endingTagSetsFile.c_str());
		}

		void FormsIdentifier::Identify(std::string form, std::vector<CToken*>& out)
		{
			std::string encodedForm = Parser::Convert(form,this->inputEncoding, Parser::cp1250);
			Morfeusz::Analyse(out,encodedForm);

			for(int i = 0; i < (int)out.size(); i++)
			{
				if(guesser != NULL && guesserUsage == guessAll
					&& out[i]->GetLexCount() == 1 && out[i]->GetLexem(0)->GetTag().FlexClass == cf_ign)
				{
					CToken* temp = out[i];
					out[i] = this->guesser->Guess(out[i]->GetOrth().c_str(),true);
					delete temp;
				}

				out[i]->SetOrth(Parser::Convert(out[i]->GetOrth(),Parser::cp1250, outputEncoding));
				for(int j = 0; j < (int)out[i]->GetLexCount(); j++)
				{
					out[i]->GetLexem(j)->SeBase(Parser::Convert(out[i]->GetLexem(j)->GetBase(),Parser::cp1250, outputEncoding));
				}
			}
		}

		void FormsIdentifier::IdentifyNoConv(std::string cp_form, std::vector<CToken*>& out)
		{
			Morfeusz::Analyse(out, cp_form);

			if(guesser != NULL && guesserUsage == guessAll)
			{
				for(int i = 0; i < (int)out.size(); i++)
				{
					if(out[i]->GetLexCount() == 1 && out[i]->GetLexem(0)->GetTag().FlexClass == cf_ign)
					{
						CToken* temp = out[i];
						out[i] = this->guesser->Guess(out[i]->GetOrth().c_str(),true);
						delete temp;
					}
				}
			}
		}

		void FormsIdentifier::FreeTokenVector(std::vector<CToken*> &tokens)
		{
			for(size_t i = 0; i < tokens.size(); i++)
			{
				Free(tokens[i]);
			}
			tokens.clear();
		}
	}
}
