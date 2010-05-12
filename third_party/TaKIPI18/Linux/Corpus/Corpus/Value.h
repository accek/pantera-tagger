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

#ifndef CORPUSLIB_VALUE_H
#define CORPUSLIB_VALUE_H

#include "../corpuslib_headers.h"
#include "typedefs.h"

namespace Corpus
{
	/**
	Klasa s³u¿¹ca do przechowywania pary wartoœci, reprezentuj¹cych bitowy zapis &lt;klasaFleksyjna,wartoœciKategoriiGramatycznych&gt;.
	W specjalnym przypadku mo¿ê s³u¿yæ do przechowywania ³añcucha znaków (&lt;indeks£añcuchawS³owniku,sta³aStringowa&gt;).
	Ponadto zaimplementowane zosta³y metody do obs³ugiwania wartoœci. */
	class CORPUS_API CValue
	{
	public:
		/// Tworzy "pust¹" wartoœæ.
		CValue() { first = 0; second = 0; }

		/// Konstrutkro kopiuj¹cy.
		CValue(const CValue& val) { first = val.first; second = val.second; }

		/// Konstuktor tworz¹cy obiekt &lt;f,s&gt;.
		CValue(DLONG f, DLONG s);
		~CValue(void);

		/**
		Operator porównania (mniejszoœci) porównuje najpierw wartoœci first,
		a jeœli s¹ równe to wartoœci second (s³uzy do sortowania). */
		const bool operator<(const CValue& v) const
		{
			if(this->first < v.first)
				return true;
			else if(this->first > v.first)
				return false;
			else
				return this->second < v.second;
		}

		/// Operator porównania.
		const bool operator==(const CValue& v) const
		{
			return first == v.first && second == v.second;
		}

		/// Operator nierównoœci.
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
		Funkcja sprawdzaj¹ca czy akturalnie przechowywana wartoœci jest reprezentacj¹
		³añcucha znaków. */
		inline bool IsString()
		{
			return (second & str_msk) != 0;
		}

		/**
		Funkcja sprawdzaj¹ca czy aktualnie przechowywana wartoœci jest reprezenacj¹
		jakieœ klasy fleksyjnej (abstrahuj¹c od kategorii gramatycznych). */
		inline bool IsPos()
		{
			if(IsString())
				return false;
			else
				return (first > 0);
		}

		/** Zwraca wartoœci atrybutów jeœli wartoœæ opisuje tag
		  * lub indeks w s³owniku jeœli opisuje wartoœæ typu string.
		  * Uwaga: jeœli opisuje nieokreœlon¹ wartoœæ typu string,
		  * tj. o indeksie 0 w s³owniku, funkcja zwróci sta³¹
		  * <CODE>str_msk</CODE>. */
		inline DLONG GetPatternValue()
		{
			if(IsString() || first > 0)
				return first;
			else 
				return second;
		}

		/*
		Dwie wartoœci tworz¹cê klasê.
		*/
		DLONG first;
		DLONG second;
	};
}
#endif

