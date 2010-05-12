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

#ifndef CORPUSLIB_EXTSENTENCER_LOWLEVELSENTENCER_H
#define CORPUSLIB_EXTSENTENCER_LOWLEVELSENTENCER_H

#include "../../corpuslib_headers.h"
#include "../typedefs.h"
#include "TokenBuffer.h"
#include "../Parser/Parser.h"

#define DECSN_EOT (-1)
#define DECSN_NOTHING (-2)

namespace Corpus
{
	namespace ExtSentencer
	{
		/** Low-level sentencer class. Operates on strings, returns decisions.
		  * Can be employed directly to bind with another module, reducing
		  * overhead of token buffering.
		  * To generate sentences, i.e. containers of KIPI tokens, use the
		  * Sentencer class, which supports token buffering and more convenient
		  * API. */
		class CORPUS_API CLowLevelSentencer
		{
		public:
			CLowLevelSentencer();

			/** By default, the sentencer uses CConfig::inputEncoding. Another
			  * encoding can be set if needed. */
			void SetNonstandardEncoding(Parser::Encoding newEncoding)
			{
				specialEncoding = newEncoding;
				useSpecialEncoding = true;
			}

			/// Returns to default use of CConfig::inputEncoding.
			void DisableNonstandardEncoding()
			{
				useSpecialEncoding = false;
			}

			/** Feeds the sentencer with the next token. A decision is returned
			  * i.e.
			  * a) a positive integer meaning that a new sentence has been
			  * detached (the return value is then the number of tokens
			  * constituting the sentence),
			  * b) DECSN_NOTHING if nothing happened or
			  * c) DECSN_EOT when the end of input has been reached. */
			INTEGER Feed(const char *co_orth, const bool wasNS, const bool isSym);
			
			/** Sends an end-of-input signal to the sentencer. Returns the last
			  * decision. */
			INTEGER Fed();
			
			/** Resets the sentencer's internal state
			  * (it will be as brand new). */
			void Reset();

		private:
			CTokenBuffer buffer;
			INTEGER currentSentTokens;
			Parser::Encoding specialEncoding;
			bool useSpecialEncoding;
			
			static bool IsDot(const char *cp_orth)
			{
				return (cp_orth != NULL
					&& cp_orth[0] == '.'
					&& cp_orth[1] == 0);
			}

			static bool IsExcOrQuest(const char *cp_orth)
			{
				return (cp_orth != NULL
					&& (cp_orth[0] == '?' || cp_orth[0] == '!')
					&& cp_orth[1] == 0);
			}

			static bool IsSentEndMarker(const char *cp_orth)
			{
				return (cp_orth != NULL
					&& (cp_orth[0] == '.' || cp_orth[0] == '?' || cp_orth[0] == '!')
					&& cp_orth[1] == 0);
			}

			static bool StartsWithNotLower(const std::string *cp_orth)
			{
				return (cp_orth == NULL || cp_orth->size() == 0
					|| !Parser::IsLowerCP1250(cp_orth->operator [](0)));
			}

			INTEGER Decide();
			INTEGER DecideOnInterp(const std::string *cp_preOrth,
				const std::string *cp_interpOrth,
				const std::string *cp_postOrth,
				bool preInterpNS, bool postInterpNS);
			INTEGER DecideOnSym(const std::string *cp_preOrth,
				const std::string *cp_interpOrth,
				const std::string *cp_postOrth,
				bool preInterpNS, bool postInterpNS);

			INTEGER DetachNow()
			{
				INTEGER tokensToFlush = currentSentTokens - buffer.GetLength() + 2;
				currentSentTokens -= tokensToFlush;

				return tokensToFlush;
			}
		}; // class CLowLevelSentencer

	} // namespace ExtSentencer
} // namespace Corpus

#endif
