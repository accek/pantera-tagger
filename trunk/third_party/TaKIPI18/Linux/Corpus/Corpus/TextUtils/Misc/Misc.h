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

#ifndef CORPUS_TEXTUTILS_MISC_H
#define CORPUS_TEXTUTILS_MISC_H

#include "../../../corpuslib_headers.h"
#include "../../WordsDictionary.h"

namespace Corpus
{
	namespace TextUtils
	{
		namespace Misc
		{
			void CORPUS_API ReadWords(std::set<std::string> & words, std::string filename) throw(std::string);
			void CORPUS_API ReadWords(std::set<std::string> & words, std::string filename, std::string stoplist_filename) throw(std::string);

			/**
			Wczytuje slowa, odrzuca duplikaty, sortuje.
			*/
			void CORPUS_API ReadWords(std::vector<std::string>&words, std::string filename);
			/**
			Wczytuje slowa, odrzuca duplikaty, sortuje.
			*/
			void CORPUS_API ReadWords(std::vector<std::string>&words, std::string filename, std::string stoplist_filename);
			void CORPUS_API ReadIntList(std::vector<int> & values, std::string filename) throw(std::string);
			void CORPUS_API ReadIntList(std::vector<size_t> & values, std::string filename) throw(std::string);

			void CORPUS_API ReadWords(std::set<int> & words, std::string filename, Corpus::CWordDictionary * dict) throw(std::string);

			void CORPUS_API SaveWords(std::set<int> & words, std::string filename, Corpus::CWordDictionary * dict) throw(std::string,Corpus::tagexception);
			void CORPUS_API SaveWords(std::set<std::string> & words, std::string filename) throw(std::string);
			void CORPUS_API SaveWords(std::vector<std::string> & words, std::string filename);

			void CORPUS_API FilterWords(std::set<std::string> & words, std::set<std::string> & stoplist);

		} // namespace Misc
	} // namespace TextUtils
} // namespace Corpus

#endif
