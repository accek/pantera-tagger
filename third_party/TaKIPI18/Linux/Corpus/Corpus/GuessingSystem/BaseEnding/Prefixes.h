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

#ifndef CORPUSLIB_PREFIXES_H
#define CORPUSLIB_PREFIXES_H

#include "../../../corpuslib_headers.h"
#include "../../Config.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace BaseEnding
		{
			class CORPUS_API Prefixes
			{
			public:
				/// Realizuje heurystyk� opart� o wybrane przedrostki. Doprowadza form� do "zniekszta�conej" postaci.
				std::string ProcessPrefixes(std::string cp_lowerform);
			
				static Prefixes *Deserialize(std::string filename = CConfig::GetPrefixPath());
				
				/// Statyczny dost�p do instancji (singleton).
				static Prefixes *GetInstance();
				
			private:
				std::vector<std::string> prefixes;
			};
		}
	}
}

#endif
