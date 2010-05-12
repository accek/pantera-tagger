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

#ifndef CORPUSLIB_READER_H
#define CORPUSLIB_READER_H

#include "../../corpuslib_headers.h"
#include "../GuessingSystem/FormsIdentifier.h"
#include "../Token.h"


namespace Corpus
{
	namespace Readers
	{
		/**
		Podstawa dla ka¿dego z czytaczy. Poprawka: CReader::Clean() jest ju¿
		niepotrzebne (pozostawione aby utrzymaæ zgodnoœæ z innymi projektami).
		*/
		class CORPUS_API CReader
		{
		public:

			CReader(std::istream& inStream);
			virtual ~CReader(void);

			virtual CToken* GetToken() = 0;
			std::string GetNextTokenText(std::istream& inStream);

			/// \todo to nie powinno byæ publiczne!
			std::string befTextTags;
			std::string lastXmlTag;
			bool newTag;
			bool forceUtfEncoding;

			/// NIEPOTRZEBNE, nic nie robi.
			static void CleanUp();
			

		protected:
			
			std::istream& stream;
			std::stringstream bufferStream_;
		};
	}
}

#endif

