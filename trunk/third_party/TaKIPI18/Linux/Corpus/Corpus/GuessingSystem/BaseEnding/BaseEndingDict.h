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

#ifndef CORPUSLIB_BASEENDINGDICT_H
#define CORPUSLIB_BASEENDINGDICT_H

#include "../../../corpuslib_headers.h"

#include "../../typedefs.h"
#include "../../tagexception.h" // TODO: zrobiæ bardziej sensowne wyj¹tki, to kalka z CWordDictionary

#include "../../Config.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace BaseEnding
		{
			/** S³ownik koñcówek form bazowych. Dziêki niemu w EndingTagSet::TagCount trzymany jest indeks (int)
			koñcówki bazowej zamiast ca³ego ci¹gu znaków. Klasa wzorowana jest na CWordDictionary. */
			class CORPUS_API BaseEndingDict
			{
			public:
				/// Destruktor.
				~BaseEndingDict();

				// Czyœci s³ownik.
				void Clear()
				{
					stringVect->clear();
					string2int->clear();
				}

				/// Statyczny dostêp do instancji (dostêp z zewsz¹d do jednego s³ownika -- singleton).
				static BaseEndingDict *GetInstance();

				/** Deserializaja s³ownika (metoda statyczna). Odtwarza instancjê singletona.
				Flaga writeMsg, mówi czy wyœwietlaæ informacje o postêpie wczytywania na konsolê.
				Zwracana jest instancja (równowa¿ne póŸniejszemu wywo³aniu GetInstance).
				*/
				static BaseEndingDict* Deserialize(std::string filename = CConfig::GetBaseEndingDictPath());

				/// Serializacja s³ownika (metoda statyczna). Zapisuje instancjê s³ownika do podanego pliku.
				static bool Serialize(std::string filename = CConfig::GetBaseEndingDictPath());

				/// Sprawdza czy s³ownik zawiera podany ci¹g.
				bool Contains(const std::string &s)
				{
					return (string2int->find(s) != string2int->end());
				}

				/// Zwraca ci¹g zawarty pod adresem i. Rzuci wyj¹tek, gdy nie znajdzie.
				const std::string& GetAt(unsigned int i);

				/// Zwraca adres ci¹gu s. Jeœli nie znajdzie, zwróci wartoœæ -1.
				int GetIdx(const std::string &s)
				{
					STRING2INT_MAP::iterator it = string2int->find(s);
					if(it == string2int->end())
						return -1;
					else
						return it->second;
				}

				/// Jeœli ci¹gu s nie ma w s³owniku, zostanie dodany. Tak czy siak, funkcja zwraca indeks ci¹gu.
				int Add(const char *s);

				/// Zwraca iloœæ wyrazów w s³owniku.
				int Count()
				{
					return (int)stringVect->size();
				}

			private:
				///	Konstruktor tworz¹cy pusty s³ownik.
				BaseEndingDict();

				/// Wektor stringów (rzutuje int na string).
				STRING_VECTOR *stringVect;
				/// Odzworowanie <int,string> (rzutuje string na int).
				STRING2INT_MAP	*string2int;
			};
		}
	}
}

#endif
