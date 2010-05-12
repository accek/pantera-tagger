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

#ifndef CORPUSLIB_WORDSDICTIONARY_H
#define CORPUSLIB_WORDSDICTIONARY_H

#include "../corpuslib_headers.h"
#include "tagexception.h"
#include "typedefs.h"

namespace Corpus
{
	/**
	S�ownik s��w, kt�rego utworzenie wi�za�o si� z konieczno�ci� zmniejszenia
	rozmiaru serializowanych danych oraz przy�pieszenia dzia�ania poprzez przekazywanie
	tylko indeksu s�owa (int), a nie �a�cucha znak�w.
	*/
	class CORPUS_API CWordDictionary
	{
	public:
		/// Sprawdza, czy s�ownik zawiera podany string.
		bool Contains(const std::string &s);

		/// Sprawdza, czy s�ownik zawiera podany string.
		bool Contains(const _TCHAR* s);

		/// Sprawdza czy w s�owniku znajduje si� indeks o podanym indeksie.
		bool Contains(const unsigned int i);

		/**Pobiera stringa zawartego pod indeksem i jak nie znajdzie,
		rzuca wyjatek. */
		const std::string& GetAt(unsigned int i);

		/** Pobiera index, pod kt�rym znajduje si� zadane parametrem s�owo;
		zwraca -1 gdy nie znaleziono. */
		int GetIdx(const std::string &s);

		/**
		Dodaje podany string do s�ownika, o ile go tam nie ma zwraca index wyrazu,
		niezale�nie czy �wie�o dodanego, czy ju� si� znajduj�cego. */
		int Add(const std::string &s);

		/** Dodaje ca�y s�ownik */
		void Add(CWordDictionary* dict);

		/** Serializacja s�ownika do pliku. */
		void Serialize(std::ostream& file);
		/** Deserializaja s�ownika (metoda statyczna).
		Wczytuje s�onik ze strumienia wed�ug schematu zapisywania.
		Flaga writeMsg, m�wi czy wy�wietla� informacje o post�pie wczytywania na konsol�.
		Zwracany jest utworzony z pliku s�ownik s��w. */
		static CWordDictionary* Deserialize(std::istream& file, bool writeMsg = true, bool clearDict = true);
		
		/// Serializuje w postaci pliku tekstowego (sluzy do przenoszenia miedzy platformami).
		void SerializeAsText(std::ostream &file);
		/// Deserializuje z pliku tekstowego.
		static CWordDictionary* DeserializeFromText(std::istream& file, bool writeMsg = true, bool clearDict = true);

		/// Zwraca ilo�� wyraz�w w s�owniku.
		int Count();

		/// Czy�ci s�ownik.
		void Clear();

		~CWordDictionary(void);

		/// Statyczny s�ownik (by mie� zewsz�d dost�p).
		static CWordDictionary *GetInstance();

		/** Tworzy now� instancj�, kt�r� TRZEBA zwolni� poprzez
		  * ReleaseExternalInstance -- nie b�dzie ona singletonem!
		  * Do u�ytku jako pomocniczny s�ownik do dowolnych cel�w. */
		static CWordDictionary *CreateExternalInstance();

		/** Zwalnia pami�� pomocniczej instancji s�ownika. */
		static void ReleaseExternalInstance(CWordDictionary *extInstance);
		

	private:
		/*
		Konstruktor tworz�cy pusty s�ownik.
		*/
		CWordDictionary(void);

		/*
		Wektor string�w (rzutuje int na string)
		*/
		STRING_VECTOR	*stringVect;
		/*
		Mapa <int,string> (rzutuje string na int)
		*/
		STRING2INT_MAP	*string2int;
	};

}
#endif

