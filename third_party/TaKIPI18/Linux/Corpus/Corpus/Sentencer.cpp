#include "../corpuslib_headers.h"
#include "Sentencer.h"

namespace Corpus
{

	CSentencer::CSentencer(Corpus::Readers::CReader& reader)
		:reader_(reader)
	{
		this->lastToken_ = 0;
		this->firstSentence_ = true;
		m_iLastToken = 0;

		tokr = NULL; // using an old reader
	}

	CSentencer::~CSentencer(void)
	{
	}

	int CSentencer::GetCurrentTokenNumber(CSentence * sentence)
	{
		return m_iLastToken - (sentence->Size() - sentence->GetPosition()+1);
	}

	bool CSentencer::IsSentenceEndChar(char character)
	{
		return (character == '.'
			|| character == '!'
			|| character == '?');
	}

	bool CSentencer::IsSentence(std::string befSignOrth)
	{
		std::string last = lastToken_->GetOrth();

		bool lc = !Parser::IsBig(last);
		bool il = Parser::IsPolishUTF(last.at(0)) || last.at(0) > 0 && isalnum(last.at(0));

		if(!il)
		{
			if(!befSignOrth.compare("."))
				return true;
			else
				return false;
		}

		if(lc)
			return false; //ma³a litera

		return true;
	}

	CSentence* CSentencer::GetSentence(bool eliminateSubstGerIgn)
	{
		befTextTags.clear();
		std::vector<CToken*> tokens;
		//bool first = true;

		std::string befTemp;
		if(tokr == NULL) // quick fix
		{
			befTemp = reader_.befTextTags; // gather the initial beef
		}

		if(lastToken_) // something is left unflushed
		{
			tokens.push_back(lastToken_);
			if(tokr == NULL) // quick fix
			{
				// take initial tokens and initial beef
				befTextTags += reader_.befTextTags;
			}
		}

		do // fetch tokens until end of input or a sentence is discovered
		{
			lastToken_ = ReaderGetToken(); // gets new beef as well
			if(!lastToken_)
				if( tokens.empty() ) //patch
					return NULL; // end of input, no tokens in the buffer; return nothing
				else
					break; // A: end of inputs but some tokens left

			m_iLastToken++;
			if (tokr == NULL && reader_.newTag && !firstSentence_) // quick fix
				break; // B: new tag came while we have a sentence -> detach it
			else
				firstSentence_ = false;

			if(tokr == NULL) // quick fix
			{
				befTextTags += reader_.befTextTags;
			}

			if(lastToken_)
			{
				if (tokens.size() > 1)
				{
					if(IsSentenceEndChar(tokens[tokens.size() - 1]->GetOrth()[0]))
					{
						if(IsSentence(tokens[tokens.size() - 2]->GetOrth()))
						{
							// C: SentEndChar came and the token before it satisfies IsSent
							break;
						}
					}
				}
				//std::string l = lastToken_->GetOrth();
				tokens.push_back(lastToken_);
			}
		}
		while(lastToken_);

		if(tokens.empty()) // may ever happen?
		{
			befTextTags = befTemp; // restore the initial beef
			return 0; // and return
			// returning NULL probably means a definite end of sentences...
			// is this behaviour intended? (if it happened in case C for instance)
		}
		else
		{
			// append all end sentence chars to current sentence (quick fix)
			while(lastToken_ && IsSentenceEndChar(lastToken_->GetOrth()[0]) )
			{
				tokens.push_back(lastToken_);
				lastToken_ = ReaderGetToken();
			}

			CSentence *sentence = new CSentence(tokens);
			if(eliminateSubstGerIgn)
			{
				sentence->EliminateSubstGerIgn();
			}
			
			return sentence; // all beef now comes from non-initial calls
		}
	} // GetSentence

	/// Quick fix: use new tokeniser.
	CSentencer::CSentencer(Corpus::Tokenisation::CTokeniser *tokeniser)
		:reader_(emptyReader) // quick fix!
	{
		this->lastToken_ = 0;
		this->firstSentence_ = true;
		m_iLastToken = 0;
		tokr = tokeniser;
	}

	// Quick fix: a ghost stream and reader to make the sentencer happy
	std::ifstream CSentencer::emptyStream;
	Corpus::Readers::CCorporaReader CSentencer::emptyReader(emptyStream);
} // namespace Corpus
