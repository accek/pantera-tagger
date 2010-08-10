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

#ifndef CORPUSLIB_CONFIG_H
#define CORPUSLIB_CONFIG_H

#include "../corpuslib_headers.h"
#include "Parser/Parser.h"
#include "GuessingSystem/FormsIdentifier.h"

namespace Corpus
{
	class CORPUS_API CConfig
	{
	public:
		enum FileType
		{
			text,
			corpora, // jezeli inputType=corpora, to nie ma tagow disamb (inputtype jest sprawdzane tylko w CToken )
			preMorph
		};

		static TFlexClass flexClassesToGuess;

		static Parser::Encoding inputEncoding;
		static FileType inputType; // uzywane w CPlainTextReader
		
		static int guesserUsage;

		static void SetPathsToDirectory(const std::string &dir);
		static std::string GetFullPath(std::string& filename);
		
		static std::string GetEndingTreePath()
		{
			return GetFullPath(endingTreePath);
		}
		static std::string GetEndingTagSetPath()
		{
			return GetFullPath(endingTagSetPath);
		}
		static std::string GetDictionaryPath()
		{
			return GetFullPath(dictionaryPath);
		}
		static std::string GetTestLogPath()
		{
			return GetFullPath(testLogPath);
		}
		static std::string GetBaseEndingDictPath()
		{
			return GetFullPath(baseEndingDictPath);
		}
		static std::string GetAbbrListPath()
		{
			return GetFullPath(abbrListPath);
		}
		static std::string GetAbbrSymListPath()
		{
			return GetFullPath(abbrSymListPath);
		}
		static std::string GetTokeniserRulePath()
		{
			return GetFullPath(tokeniserRulePath);
		}
		static std::string GetTokeniserSepPath()
		{
			return GetFullPath(tokeniserSepPath);
		}
		static std::string GetPrefixPath()
		{
			return GetFullPath(prefixPath);
		}
		
	private:
		static std::string directories;
		
		// dla Guessera: drzewo, slownik, wyjscie z testow
		static std::string endingTreePath;
		static std::string endingTagSetPath;
		static std::string dictionaryPath;
		static std::string testLogPath;
		// dla Guessera: koncowki form bazowych
		static std::string baseEndingDictPath;
		// dla ExtSentencera: slownik skrotow
		static std::string abbrListPath;
		static std::string abbrSymListPath;
		// dla tokenizatora: reguly
		static std::string tokeniserRulePath;
		static std::string tokeniserSepPath;
		// dla odgadywacza: prefiksy
		static std::string prefixPath;
	};
}
#endif

