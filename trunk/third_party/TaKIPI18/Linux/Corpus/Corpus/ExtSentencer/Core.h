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

#ifndef CORPUSLIB_EXTSENTENCER_CORE_H
#define CORPUSLIB_EXTSENTENCER_CORE_H

#include "../../corpuslib_headers.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		/** The common part for all instances of ExtSentencer::CSentencer.
		  */
		class CORPUS_API CCore
		{
		public:
			CCore();
			bool IsTokAbbrev(const std::string &cp_orth);
			bool IsSymAbbrev(const std::string &cp_orth);

		private:
			void LoadAbbrevList();
			std::set<std::string> tok_abbrs;
			std::set<std::string> sym_abbrs;

			static bool LoadList(const std::string &path, std::set<std::string> &list);
		};

	} // namespace ExtSentencer
} // namespace Corpus

#endif
