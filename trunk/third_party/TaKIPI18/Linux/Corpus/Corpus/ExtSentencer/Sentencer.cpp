#include "../../corpuslib_headers.h"
#include "Sentencer.h"
#include "../typedefs.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		CSentencer::CSentencer(Tokenisation::CTokenReader &tokreader)
				:worker(), tokrdr(&tokreader)
		{
			worker.SetNonstandardEncoding(Corpus::Parser::cp1250);
		}

		CSentencer::~CSentencer()
		{
		}
		
		CSentence *CSentencer::GetNext(bool eliminateSubstGerIgn)
		{
			CToken *tok;
			INTEGER decision;
			
			// fetch tokens while they are there
			while((tok = tokrdr->GetNext()) != NULL)
			{
				tokQueue.push(tok);
				
				bool isSym = (tok->GetLexCount() > 0)
					&& (tok->GetLexem(0)->GetTag().FlexClass == cf_tsym);

				decision = worker.Feed(tok->GetOrth().c_str(), tok->NS, isSym);
				if(decision <= 0)
				{
					if(decision != DECSN_NOTHING)
					{
						throw tagexception("Sentencer output terminated before end of tokens");
					} // else nothing happens, fetch next token
				}
				else // a sentence is being detached
				{
					return Detach(decision, eliminateSubstGerIgn);
				}
			}
			while((decision = worker.Fed()) != DECSN_EOT)
			{
				if(decision > 0)
				{
					return Detach(decision, eliminateSubstGerIgn);
				}
			}
			// flush the remainder to a sentence
			return Detach((INTEGER)tokQueue.size(), eliminateSubstGerIgn);
		} // GetNext
		
		Corpus::CSentence *CSentencer::Detach(INTEGER numTokens, bool eliminateSubstGerIgn)
		{
			if(numTokens > 0)
			{
				Corpus::CSentence *sent = new Corpus::CSentence();
				/*
				std::cout << "####### SENTENCE of "
					<< numTokens << " while queue of "
					<< tokQueue.size() << "\n";
				*/
				for(INTEGER count = 0; count < numTokens; count++)
				{
					CToken *tok = tokQueue.front();
					tokQueue.pop();
					sent->AddToken(tok);
				}
				if(eliminateSubstGerIgn)
				{
					sent->EliminateSubstGerIgn();
				}

				return sent;
			}
			return NULL;
		}
		
	} // namespace ExtSentencer
} // namespace Corpus
