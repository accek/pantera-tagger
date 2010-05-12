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

#ifndef CORPUSLIB_VALUE_H
#define CORPUSLIB_VALUE_H

#include "../corpuslib_headers.h"
#include "typedefs.h"

namespace Corpus
{
	/**
	Klasa s�u��ca do przechowywania pary warto�ci, reprezentuj�cych bitowy zapis &lt;klasaFleksyjna,warto�ciKategoriiGramatycznych&gt;.
	W specjalnym przypadku mo�� s�u�y� do przechowywania �a�cucha znak�w (&lt;indeks�a�cuchawS�owniku,sta�aStringowa&gt;).
	Ponadto zaimplementowane zosta�y metody do obs�ugiwania warto�ci. */
	class CORPUS_API CValue
	{
	public:
		/// Tworzy "pust�" warto��.
		CValue() { first = 0; second = 0; }

		/// Konstrutkro kopiuj�cy.
		CValue(const CValue& val) { first = val.first; second = val.second; }

		/// Konstuktor tworz�cy obiekt &lt;f,s&gt;.
		CValue(DLONG f, DLONG s);
		~CValue(void);

		/**
		Operator por�wnania (mniejszo�ci) por�wnuje najpierw warto�ci first,
		a je�li s� r�wne to warto�ci second (s�uzy do sortowania). */
		const bool operator<(const CValue& v) const
		{
			if(this->first < v.first)
				return true;
			else if(this->first > v.first)
				return false;
			else
				return this->second < v.second;
		}

		/// Operator por�wnania.
		const bool operator==(const CValue& v) const
		{
			return first == v.first && second == v.second;
		}

		/// Operator nier�wno�ci.
		const bool operator!=(const CValue& v) const
		{
			return first != v.first || second != v.second;
		}

		/// Operator przypisania.
		CValue& operator=(const CValue& v)
		{
			this->first = v.first;
			this->second = v.second;

			return *this;
		}

		/**
		Funkcja sprawdzaj�ca czy akturalnie przechowywana warto�ci jest reprezentacj�
		�a�cucha znak�w. */
		inline bool IsString()
		{
			return (second & str_msk) != 0;
		}

		/**
		Funkcja sprawdzaj�ca czy aktualnie przechowywana warto�ci jest reprezenacj�
		jakie� klasy fleksyjnej (abstrahuj�c od kategorii gramatycznych). */
		inline bool IsPos()
		{
			if(IsString())
				return false;
			else
				return (first > 0);
		}

		/** Zwraca warto�ci atrybut�w je�li warto�� opisuje tag
		  * lub indeks w s�owniku je�li opisuje warto�� typu string.
		  * Uwaga: je�li opisuje nieokre�lon� warto�� typu string,
		  * tj. o indeksie 0 w s�owniku, funkcja zwr�ci sta��
		  * <CODE>str_msk</CODE>. */
		inline DLONG GetPatternValue()
		{
			if(IsString() || first > 0)
				return first;
			else 
				return second;
		}

		/*
		Dwie warto�ci tworz�c� klas�.
		*/
		DLONG first;
		DLONG second;
	};
}
#endif

