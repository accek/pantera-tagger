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

#ifndef CORPUSLIB_POSIOTION_H
#define CORPUSLIB_POSIOTION_H

#include "../corpuslib_headers.h"
namespace Corpus
{

	class CORPUS_API CPosition
	{
	public:
		CPosition(int position = CPosition::nowhere);
		virtual ~CPosition(void);

		virtual int GetPos() const;

		virtual void SetPos(int newPosition);

		std::string ToString();

		enum Position
		{
			current = 0,
			beginning = INT_MIN+1,
			end = INT_MAX,
			nowhere = INT_MIN
		};

	protected:
		// Pozycja wskazywana przez klas�.
		int position;
	};
}

#endif

