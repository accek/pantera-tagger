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

#ifndef CORPUSLIB_EXTSENTENCER_DEVEL_DEVEL_H
#define CORPUSLIB_EXTSENTENCER_DEVEL_DEVEL_H

#include "../../../corpuslib_headers.h"
#include "../../Token.h"
#include "../../Lexem.h"


namespace Corpus
{
	namespace ExtSentencer
	{
		namespace Devel
		{
			class AbbrevExtractor
			{
			public:
				AbbrevExtractor(std::string morphFile, std::string prefix,
					bool retokeniser = false)
					:morphFN(morphFile), prefixFN(prefix)
				{
				}

				void Extract();
				void WriteAbbrList(const std::string &filename)
				{
					Write(filename, lst_abbr);
				}

				void WriteNotAbbrList(const std::string &filename)
				{
					Write(filename, lst_notabbr);
				}

				void WriteDiffList(const std::string &filename);

				static bool IsDot(CToken *tok);
				static bool IsInterp(CToken *tok);
				static bool IsNDInterp(CToken *tok);
				static void RunAll(const std::string &pseudococonfigPath);

			private:
				std::string morphFN, prefixFN;
				std::map<std::string, size_t> lst_abbr, lst_notabbr;
				std::vector<CToken *> lastGroup;

				void HandleGroup();
				void HandleGroupRetok();
				void Write(const std::string &outFileName, bool corruptedList);
				void Insert(int beg, int end,
					std::map<std::string, size_t> &collection);

				static void Write(const std::string &outFileName,
					std::map<std::string, size_t> &collection);
			};

		}// namespace Devel
	} // namespace ExtSentencer
} // namespace Corpus
#endif
