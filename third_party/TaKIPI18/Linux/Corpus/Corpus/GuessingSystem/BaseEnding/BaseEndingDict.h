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

#ifndef CORPUSLIB_BASEENDINGDICT_H
#define CORPUSLIB_BASEENDINGDICT_H

#include "../../../corpuslib_headers.h"

#include "../../typedefs.h"
#include "../../tagexception.h" // TODO: zrobi� bardziej sensowne wyj�tki, to kalka z CWordDictionary

#include "../../Config.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace BaseEnding
		{
			/** S�ownik ko�c�wek form bazowych. Dzi�ki niemu w EndingTagSet::TagCount trzymany jest indeks (int)
			ko�c�wki bazowej zamiast ca�ego ci�gu znak�w. Klasa wzorowana jest na CWordDictionary. */
			class CORPUS_API BaseEndingDict
			{
			public:
				/// Destruktor.
				~BaseEndingDict();

				// Czy�ci s�ownik.
				void Clear()
				{
					stringVect->clear();
					string2int->clear();
				}

				/// Statyczny dost�p do instancji (dost�p z zewsz�d do jednego s�ownika -- singleton).
				static BaseEndingDict *GetInstance();

				/** Deserializaja s�ownika (metoda statyczna). Odtwarza instancj� singletona.
				Flaga writeMsg, m�wi czy wy�wietla� informacje o post�pie wczytywania na konsol�.
				Zwracana jest instancja (r�wnowa�ne p�niejszemu wywo�aniu GetInstance).
				*/
				static BaseEndingDict* Deserialize(std::string filename = CConfig::GetBaseEndingDictPath());

				/// Serializacja s�ownika (metoda statyczna). Zapisuje instancj� s�ownika do podanego pliku.
				static bool Serialize(std::string filename = CConfig::GetBaseEndingDictPath());

				/// Sprawdza czy s�ownik zawiera podany ci�g.
				bool Contains(const std::string &s)
				{
					return (string2int->find(s) != string2int->end());
				}

				/// Zwraca ci�g zawarty pod adresem i. Rzuci wyj�tek, gdy nie znajdzie.
				const std::string& GetAt(unsigned int i);

				/// Zwraca adres ci�gu s. Je�li nie znajdzie, zwr�ci warto�� -1.
				int GetIdx(const std::string &s)
				{
					STRING2INT_MAP::iterator it = string2int->find(s);
					if(it == string2int->end())
						return -1;
					else
						return it->second;
				}

				/// Je�li ci�gu s nie ma w s�owniku, zostanie dodany. Tak czy siak, funkcja zwraca indeks ci�gu.
				int Add(const char *s);

				/// Zwraca ilo�� wyraz�w w s�owniku.
				int Count()
				{
					return (int)stringVect->size();
				}

			private:
				///	Konstruktor tworz�cy pusty s�ownik.
				BaseEndingDict();

				/// Wektor string�w (rzutuje int na string).
				STRING_VECTOR *stringVect;
				/// Odzworowanie <int,string> (rzutuje string na int).
				STRING2INT_MAP	*string2int;
			};
		}
	}
}

#endif
