#include "../../corpuslib_headers.h"
#include "LowLevelSentencer.h"
#include "../Config.h"
#include "../ResourceManager.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		CLowLevelSentencer::CLowLevelSentencer()
			:useSpecialEncoding(false)
		{
			Reset();
		}

		void CLowLevelSentencer::Reset()
		{
			currentSentTokens = 0;
			buffer.Reset();
		}

		INTEGER CLowLevelSentencer::Feed(const char *co_orth, const bool wasNS, const bool isSym)
		{
			std::string co_inputForm(co_orth);
			Parser::Encoding fromEnc = useSpecialEncoding
				? specialEncoding : Corpus::CConfig::inputEncoding;
			std::string cp_encodedForm = Parser::Convert(co_inputForm, fromEnc, Parser::cp1250);
			buffer.Write(&cp_encodedForm, wasNS, isSym);
			currentSentTokens++;
			INTEGER decision = DECSN_NOTHING;
			if(buffer.IsFull())
			{
				decision = Decide();
			}
			
			return decision;
		}

		INTEGER CLowLevelSentencer::Fed()
		{
			return Decide();
		}

		INTEGER CLowLevelSentencer::Decide()
		{
			/* We assume that the last character has been decided earlier
			 * -- we just pass an EOT (the client is responsible for marking
			 * the remainder as the last sentence though no explicit "detach"
			 * decision has been passed). */
			INTEGER toksLeft = buffer.GetLength();
			if(toksLeft < 2)
			{
				if(toksLeft > 0)
				{
					buffer.Reset();
				}
				return DECSN_EOT;
			}

			bool wasNS0, wasNS1, wasNS2;
			bool isSym0, isSym1, isSym2;
			const std::string *cp_orth0 = buffer.GetToken(0, wasNS0, isSym0);
			const std::string *cp_orth1 = buffer.GetToken(1, wasNS1, isSym1);
			const std::string *cp_orth2 = buffer.GetToken(2, wasNS2, isSym2);

			INTEGER decision = DECSN_NOTHING;
			// only here a sentence detachment decision may be made
			if(isSym1)
				decision = DecideOnSym(cp_orth0, cp_orth1, cp_orth2, wasNS1, wasNS2);
			else if(IsSentEndMarker(cp_orth1->c_str()))
				decision = DecideOnInterp(cp_orth0, cp_orth1, cp_orth2, wasNS1, wasNS2);

			buffer.SkipOne();
			return decision;
		}

		INTEGER CLowLevelSentencer::DecideOnInterp(const std::string *cp_preOrth,
				const std::string *cp_interpOrth,
				const std::string *cp_postOrth, // moze byc NULL
				bool preInterpNS, bool postInterpNS)
		{
			// detach :- postNS(false), (Orth = '?'; Orth = '!').
			if(!postInterpNS && IsExcOrQuest(cp_interpOrth->c_str()))
				return DetachNow();

			// detach :- preNS(false), Orth = '.'.
			if(!preInterpNS && IsDot(cp_interpOrth->c_str()))
				return DetachNow();

			/* detach :-
			 *    preNS(true), postNS(false), Orth = '.',
			 *    \+ lowCase(PostOrth),
			 *    \+ abbrev(PreOrth).
			 */
			if(preInterpNS && !postInterpNS && IsDot(cp_interpOrth->c_str())
				&& StartsWithNotLower(cp_postOrth)
				&& !CResourceManager::GetInstance()->GetSentencerCore()->IsTokAbbrev(*cp_preOrth))
				return DetachNow();

			//if(cp_postOrth == NULL || StartsWithNotUpper(cp_postOrth->c_str()))
			
			// fail.
			return DECSN_NOTHING;
		}

		INTEGER CLowLevelSentencer::DecideOnSym(const std::string *cp_preOrth,
				const std::string *cp_symOrth,
				const std::string *cp_postOrth,
				bool preSymNS, bool postSymNS)
		{
			static boost::regex withdot_e(
				".*[A-Za-z\\x8a\\x8c\\x8d"
				"\\x8e\\x8f\\x9a\\x9c\\x9d\\x9e\\x9f\\xa3\\xa5"
				"\\xaa\\xaf\\xb3\\xb9\\xba\\xbc\\xbe\\xbf"
				"\\xc0-\\xd6\\xd8-\\xf6"
				"\\xf8\\xf9\\xfa\\xfb\\xfc\\xfd\\xfe]\\.");
			// detach :-
			//    postNS(false),
			//    \+ lowCase(PostOrth),
			//    abbrev_match(Orth1),
			//    \+ abbrev_sym(Orth1).
			if(!postSymNS && StartsWithNotLower(cp_postOrth)
				&& boost::regex_match(*cp_symOrth, withdot_e)
				&& !CResourceManager::GetInstance()->GetSentencerCore()->IsSymAbbrev(*cp_symOrth))
				return DetachNow();

			// fail.
			return DECSN_NOTHING;
		}
	}
}
