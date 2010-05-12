/*  Biblioteka Corpus
 *  Zestaw narzędzi do przetwarzania tekstów w języku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef CORPUSLIB_SENTENCER_H
#define CORPUSLIB_SENTENCER_H

#include "../corpuslib_headers.h"
#include "Readers/Reader.h"
#include "Sentence.h"
#include "Token.h"

// experimental
#include "Tokenisation/Tokeniser.h"
#include "Readers/CorporaReader.h" // the simplest reader for ghost object

namespace Corpus
{
	/// DEPRECATED. Use ExtSentencer::CSentencer.
	class CORPUS_API CSentencer
	{
	public:
		/// Standard constructor using the old reader framework.
		CSentencer(Corpus::Readers::CReader& reader);

		/// Quick fix: an attempt to use the new tokeniser.
		CSentencer(Corpus::Tokenisation::CTokeniser *tokeniser);

		virtual ~CSentencer(void);

		CSentence* GetSentence(bool eliminateSubstGerIgn = true);
		
		/// "Beef", i.e. XML stuff which preceeded tag data in the input.
		std::string befTextTags;

		/// Contains unknown bugs... not always returs correct number!
		int GetCurrentTokenNumber(CSentence * sentence);

	private:
		bool IsSentenceEndChar(char character);
		bool IsSentence(std::string befSignOrth);

		Corpus::Readers::CReader& reader_;
		CToken*  lastToken_;
		bool firstSentence_;

		int m_iLastToken;

		Corpus::Tokenisation::CTokeniser *tokr;

		static std::ifstream emptyStream;
		static Corpus::Readers::CCorporaReader emptyReader;

		Corpus::CToken *ReaderGetToken()
		{
			if(tokr == NULL)
			{
				return reader_.GetToken();
			}
			else
			{
				// tokCopy = tok->CreateHardCopy; CTokeniser::Free(tok) ???
				// it seems not necessary as delete will be issued inside
				// CSentence, which is in the same library
				// (the same memory model)
				return tokr->GetNext();
			}
		}
	};
}
#endif

