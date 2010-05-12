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
				/// Realizuje heurystykê opart¹ o wybrane przedrostki. Doprowadza formê do "zniekszta³conej" postaci.
				std::string ProcessPrefixes(std::string cp_lowerform);
			
				static Prefixes *Deserialize(std::string filename = CConfig::GetPrefixPath());
				
				/// Statyczny dostêp do instancji (singleton).
				static Prefixes *GetInstance();
				
			private:
				std::vector<std::string> prefixes;
			};
		}
	}
}

#endif
