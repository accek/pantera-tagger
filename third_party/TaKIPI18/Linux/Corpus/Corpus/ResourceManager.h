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

#ifndef CORPUSLIB_RESOURCEMANAGER_H
#define CORPUSLIB_RESOURCEMANAGER_H

#include "../corpuslib_headers.h"
#include "Config.h"
#include "GuessingSystem/FormsIdentifier.h"
#include "ExtSentencer/Core.h"
#include "ExtSentencer/LowLevelSentencer.h"
#include "Tokenisation/Core.h"
#include "Morfeusz/morfeusz.h"

namespace Corpus
{
	/** Klasa pomocnicza ulatwiajaca zarzadzanie zasobami. Pozwala
	  * na stworzenie automatycznego obiektu, ktorego konstruktor
	  * zwolni zaladowane zasoby. Obiekt tworzony jest w momencie,
	  * gdy jest potrzebny (wywolanie Get).
	  */
	class CORPUS_API CResourceManager
	{
	public:
		static CResourceManager *GetInstance() { return &instance; }

		CResourceManager(): fi(NULL), sentrCore(NULL)
		{
			// Troche brzydki zabieg, ale dzieki temu bedzie to wywolywane zawsze
			// -- obiekt instance tworzony jest automatycznie.
			// Morfeusz jest zasobem wspoldzielonym, jakies kodowanie trzeba ustawic;
			// poki co jest to CP1250, w przyszlosci ma to byc UTF-8 (gdy bedzie wspolpraca z ICU)
			Morfeusz::SetEncoding(MORFEUSZ_CP1250);
		}

		~CResourceManager()
		{
			ReleaseNow();
		}

		Corpus::GuessingSystem::FormsIdentifier *GetFormsIdentifier()
		{
			if(fi == NULL)
				fi = new GuessingSystem::FormsIdentifier(CConfig::inputEncoding,
				CConfig::inputEncoding, CConfig::guesserUsage);
			return fi;
		};

		Corpus::ExtSentencer::CCore *GetSentencerCore()
		{
			if(sentrCore == NULL)
				sentrCore = new Corpus::ExtSentencer::CCore();
			return sentrCore;
		}

		Corpus::ExtSentencer::CLowLevelSentencer *GetDefaultSentencer()
		{
			if(defaultSentr == NULL)
				defaultSentr = new Corpus::ExtSentencer::CLowLevelSentencer();
			return defaultSentr;
		}
		
		Corpus::Tokenisation::CCore *GetTokeniserCore()
		{
			if(tokrCore == NULL)
				tokrCore = new Corpus::Tokenisation::CCore();
			return tokrCore;
		}

		/** Natychmiastowo zwalnia wszystkie zasoby. Kolejne wywolanie Get
		  * spowoduje ponowne zaladowanie zasobow. */
		void ReleaseNow()
		{
			delete fi;
			fi = NULL;
			delete sentrCore;
			sentrCore = NULL;
			delete defaultSentr;
			defaultSentr = NULL;
			delete tokrCore;
			tokrCore = NULL;
		};

	private:
		static CResourceManager instance;
		Corpus::GuessingSystem::FormsIdentifier *fi;
		Corpus::ExtSentencer::CCore *sentrCore;
		Corpus::ExtSentencer::CLowLevelSentencer *defaultSentr;
		Corpus::Tokenisation::CCore *tokrCore;

	}; // CLoader

} // namespace Corpus

#endif
