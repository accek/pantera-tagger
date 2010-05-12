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

#ifndef CORPUSLIB_WORDSDICTIONARY_H
#define CORPUSLIB_WORDSDICTIONARY_H

#include "../corpuslib_headers.h"
#include "tagexception.h"
#include "typedefs.h"

namespace Corpus
{
	/**
	S³ownik s³ów, którego utworzenie wi¹za³o siê z koniecznoœci¹ zmniejszenia
	rozmiaru serializowanych danych oraz przyœpieszenia dzia³ania poprzez przekazywanie
	tylko indeksu s³owa (int), a nie ³añcucha znaków.
	*/
	class CORPUS_API CWordDictionary
	{
	public:
		/// Sprawdza, czy s³ownik zawiera podany string.
		bool Contains(const std::string &s);

		/// Sprawdza, czy s³ownik zawiera podany string.
		bool Contains(const _TCHAR* s);

		/// Sprawdza czy w s³owniku znajduje siê indeks o podanym indeksie.
		bool Contains(const unsigned int i);

		/**Pobiera stringa zawartego pod indeksem i jak nie znajdzie,
		rzuca wyjatek. */
		const std::string& GetAt(unsigned int i);

		/** Pobiera index, pod którym znajduje siê zadane parametrem s³owo;
		zwraca -1 gdy nie znaleziono. */
		int GetIdx(const std::string &s);

		/**
		Dodaje podany string do s³ownika, o ile go tam nie ma zwraca index wyrazu,
		niezale¿nie czy œwie¿o dodanego, czy ju¿ siê znajduj¹cego. */
		int Add(const std::string &s);

		/** Dodaje ca³y s³ownik */
		void Add(CWordDictionary* dict);

		/** Serializacja s³ownika do pliku. */
		void Serialize(std::ostream& file);
		/** Deserializaja s³ownika (metoda statyczna).
		Wczytuje s³onik ze strumienia wed³ug schematu zapisywania.
		Flaga writeMsg, mówi czy wyœwietlaæ informacje o postêpie wczytywania na konsolê.
		Zwracany jest utworzony z pliku s³ownik s³ów. */
		static CWordDictionary* Deserialize(std::istream& file, bool writeMsg = true, bool clearDict = true);
		
		/// Serializuje w postaci pliku tekstowego (sluzy do przenoszenia miedzy platformami).
		void SerializeAsText(std::ostream &file);
		/// Deserializuje z pliku tekstowego.
		static CWordDictionary* DeserializeFromText(std::istream& file, bool writeMsg = true, bool clearDict = true);

		/// Zwraca iloœæ wyrazów w s³owniku.
		int Count();

		/// Czyœci s³ownik.
		void Clear();

		~CWordDictionary(void);

		/// Statyczny s³ownik (by mieæ zewsz¹d dostêp).
		static CWordDictionary *GetInstance();

		/** Tworzy now¹ instancjê, któr¹ TRZEBA zwolniæ poprzez
		  * ReleaseExternalInstance -- nie bêdzie ona singletonem!
		  * Do u¿ytku jako pomocniczny s³ownik do dowolnych celów. */
		static CWordDictionary *CreateExternalInstance();

		/** Zwalnia pamiêæ pomocniczej instancji s³ownika. */
		static void ReleaseExternalInstance(CWordDictionary *extInstance);
		

	private:
		/*
		Konstruktor tworz¹cy pusty s³ownik.
		*/
		CWordDictionary(void);

		/*
		Wektor stringów (rzutuje int na string)
		*/
		STRING_VECTOR	*stringVect;
		/*
		Mapa <int,string> (rzutuje string na int)
		*/
		STRING2INT_MAP	*string2int;
	};

}
#endif

