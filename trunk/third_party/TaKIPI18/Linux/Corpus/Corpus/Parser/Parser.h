/*  Biblioteka Corpus
 *  Zestaw narzÄ™dzi do przetwarzania tekstÃ³w w jÄ™zyku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef CORPUSLIB_PARSER_H
#define CORPUSLIB_PARSER_H

#include "../../corpuslib_headers.h"
#include "../tagexception.h"
#include "../typedefs.h"

namespace Corpus
{
	namespace Parser
	{
		/*
		"Zjada" podany tag xml ze strumienia - najpierw go szukaj¹c
		rzuca wyj¹tkami, jeœli nie spotka taga na drodze
		np. "tag" szuka "<tag>", a "/tag" zjada "</tag>"
		zwraca to, co przeczytano do '<' bez wiodacych i konczacych spacji.
		*/
		CORPUS_API std::string EatXmlTag(std::istream &input,const char * tag, std::string& dataBeforeTag);

		/*
		Pobiera tag xml ze strumienia wejsciowego
		gotTag - jaki tag pobrano, zawartosc miedzy '<' a '>'
		dataBeforeTag - jakie dane znaleziono przed '<'
		*/
		CORPUS_API void GetXmlTag(std::istream &input,std::string& gotTag,std::string& dataBeforeTag);

		// zjada se strumienia puste znaki
		CORPUS_API void EatWhites(std::istream & input);

		/** Deprecated, use boost::algorithm::trim.
		  * usuwa z przodu i tylu wiodace spacje ze stringa i zwraca tego stringa.
		  */
		CORPUS_API std::string Trim(std::string s);

		/// Deprecated, avoid. usuwa wiod¹ce spacje z poczatku.
		CORPUS_API std::string TrimStart(std::string s);

		/// Deprecated, avoid. usuwa wiodace spacje z konca.
		CORPUS_API std::string TrimEnd(std::string s);

		//dzieli danego string'a na wyrazy w zaleznosci od podanego podzielnika
		CORPUS_API void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ");

		//Zlicza liczbê bitów w DLONG'u
		CORPUS_API int BitCount(DLONG x);

		// Dzieli zdanie na wyrazy.
		CORPUS_API void Spliter(std::string& text,std::vector<std::pair<std::string,bool> >& out);

		CORPUS_API bool IsSpace(char c, char next);

		// Kodowania znaków.
		enum Encoding
		{
			utf8 = 8,
			cp852 = 852, // unsupported by Morfeusz, avoid using it
			cp1250 = 1250,
			iso88592 = 88592
		};

		/** Podaje nazwê kodowania zgodn¹ z iconv (obs³ugiwane tylko 3 kodowania morfeuszowe). */
		CORPUS_API const char *GetEncodingName(Encoding encoding, bool translit = false);
		
		/**
			Kodowanie ³añcucha znaków o podanym kodownaiu na inne. 
		*/
		CORPUS_API std::string Convert(std::string inputString, Encoding inputEncoding, Encoding outputEncoding);

		// Metody do kodowania na UTF i z powrotem.
		CORPUS_API void ConvertFromUTFToCP1250(std::string& src, std::string& dest);
		CORPUS_API void ConvertToUTFFromCP1250(std::string& src, std::string& dest);
		
		CORPUS_API bool IsBig(std::string& word);
		CORPUS_API bool IsPolishUTF(char c);

		// Czy jest ma³¹ liter¹
		CORPUS_API bool IsLowerCP1250(char c);
		CORPUS_API bool IsUpperCP1250(char c);
		// Czy ci¹g nie zawiera ¿adnej wielkiej litery
		CORPUS_API bool IsLowerCP1250(const std::string &text);
		// Czy ci¹g nie zawiera ¿adnej ma³ej litery
		CORPUS_API bool IsUpperCP1250(const std::string &text);
		// Zamienia znak na ma³¹ literê
		CORPUS_API char ToLowerCP1250(char c);
		CORPUS_API char ToUpperCP1250(char c);
		CORPUS_API char ToLowerISO88592(char c);
		// Zamienia ci¹g znaków na ma³e litery, zwraca wskaŸnik na ten sam ci¹g
		CORPUS_API char *ToLowerCP1250(char *text);
		CORPUS_API char *ToUpperCP1250(char *text);
		CORPUS_API char *ToLowerISO88592(char *text);
		CORPUS_API bool EqualsIgnoreCaseCP1250(const std::string &s1, const std::string &s2);
		CORPUS_API void ToLowerCP1250(std::string &text);
		CORPUS_API void ToUpperCP1250(std::string &text);

		CORPUS_API DLONG ParseDLONG(std::string &text);
		CORPUS_API INTEGER ParseINTEGER(std::string &text);

		// Metody korzystaj¹ce z ICU (eksperymentalne)
		CORPUS_API void ToLowerUTF8(std::string &utf8_what);
		CORPUS_API void ToUpperUTF8(std::string &utf8_what);
	}

}
#endif

