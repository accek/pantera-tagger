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

#ifndef CORPUSLIB_CHAR_H
#define CORPUSLIB_CHAR_H

#include "../corpuslib_headers.h"
namespace Corpus
{
	class CORPUS_API CChar
	{
	public:
		CChar();
		virtual ~CChar(void);

		virtual bool IsSpace() = 0;
		virtual bool IsPunct() = 0;
		virtual void AppendToString(std::string& s) = 0;
		virtual bool IsEOF() = 0;

		virtual unsigned char operator[](int index) = 0;
		virtual int Bytes() = 0;
	};
}

#endif

