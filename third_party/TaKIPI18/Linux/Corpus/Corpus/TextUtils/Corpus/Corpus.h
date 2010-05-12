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

#ifndef CORPUS_TEXTUTILS_CORPUS_H
#define CORPUS_TEXTUTILS_CORPUS_H

#include "../../../corpuslib_headers.h"
#include "../../Corpus.h"

namespace Corpus
{
	namespace TextUtils
	{
		namespace CorpusUtil
		{
			/**
			Tworzy korpus z dokument�w otagowanych.
			*/
			void CORPUS_API CreateCorpusFromTaggedFiles(
				Corpus::CCorpus &corpus,
				std::string filename,
				std::string prefix="") throw (std::string);

		} // namespace CorpusUtil
	} // namespace TextUtils
} // namespace Corpus

#endif

