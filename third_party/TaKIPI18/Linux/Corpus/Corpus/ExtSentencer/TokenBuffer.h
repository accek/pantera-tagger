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

#ifndef CORPUSLIB_EXTSENTENCER_TOKENBUFFER_H
#define CORPUSLIB_EXTSENTENCER_TOKENBUFFER_H

#include "../../corpuslib_headers.h"
#include "../typedefs.h"
#include "../Parser/Parser.h"

namespace Corpus
{
	namespace ExtSentencer
	{

		struct STokenBufferItem
		{
			std::string cp_orth;
			bool wasNS;
			bool isSym;
		};

		/** An internal LowLevelSentencer's token buffer. Its size determines the
		  * possible look-up and look-behind. Not to be confused with the token
		  * queue used by the Sentencer class (generating KIPI sentences). */
		class CORPUS_API CTokenBuffer
		{
		public:
			CTokenBuffer()
			{
				Reset();
			}

			void Reset()
			{
				readpos = 0;
				writepos = 0;
				items = 0;
			}

			INTEGER GetLength()
			{
				return items;
			}

			INTEGER GetBufflen()
			{
				return bufflen;
			}

			bool IsFull()
			{
				return items == bufflen;
			}

			bool Write(const std::string *cp_orth, const bool wasNS, const bool isSym);
			bool SkipOne();
			const std::string *GetToken(INTEGER index, bool &wasNS, bool &isSym);
			void Dump();

		private:
			static const INTEGER bufflen = 3;
			INTEGER readpos, writepos, items;
			STokenBufferItem buff[bufflen];
		}; // CTokenBuffer

	} // namespace ExtSentencer
} // namespace Corpus
#endif
