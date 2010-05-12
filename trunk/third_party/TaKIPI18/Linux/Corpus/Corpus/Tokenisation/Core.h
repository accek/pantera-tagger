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

#ifndef CORPUSLIB_TOKENISATION_CORE_H
#define CORPUSLIB_TOKENISATION_CORE_H

#include "../../corpuslib_headers.h"
#include "Pretoken.h"
#include "../Tag.h"

namespace Corpus
{
	namespace Tokenisation
	{
		/** Classification rule of the form:
		  * classify a pretoken as flexClass with attrValues (tag)
		  * if condition (regex on its orth) satisfied. */
		struct CORPUS_API SPretokenRule
		{
			CTag tag;
			boost::regex condition;
			
			SPretokenRule(CTag aTag, std::string &regexString)
				:tag(aTag), condition(regexString)
			{
			}

			/** Tries to set pretoken's flex class and attr values,
			  * return true iff succeeded. */
			bool Classify(SPretoken &pretoken);
			
			/// Creates a classification rule from the input stream.
			static SPretokenRule *ParseContent(std::istream &content);
			/// For debugging, holds last exception string from parsing.
			static std::string lastErrorMessage;
		};
	
		/** The common part for all instances of Tokenisation::CTokeniser.
		 */
		class CORPUS_API CCore
		{
		public:
			CCore();
			~CCore();

			/// Tries to classify with all the rules.
			bool Classify(SPretoken &pretoken);

			/// If char ch is a begin separator.
			bool IsBegSep(const char ch)
			{
				return begSepMask[(unsigned char)ch];
			}

			/// If char ch is an end separator.
			bool IsEndSep(const char ch)
			{
				return endSepMask[(unsigned char)ch];
			}

			/// If char ch is an always separator.
			bool IsAlwaysSep(const char ch)
			{
				return alwSepMask[(unsigned char)ch];
			}
			
			/// If given character is blank under any (likely) encoding.
			bool IsBlankUniversal(char ch);

		protected:
			std::vector<SPretokenRule *> rules;
			// if char at i is a separator
			bool begSepMask[256], endSepMask[256], alwSepMask[256];

			void LoadClassificationRules();
			void ClearClassificationRules();
			void LoadSeparators();
		};
		
	} // namespace Tokenisation
} // namespace Corpus

#endif
