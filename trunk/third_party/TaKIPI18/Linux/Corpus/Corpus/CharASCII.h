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

#ifndef CORPUSLIB_CHARASCII_H
#define CORPUSLIB_CHARASCII_H

#include "../corpuslib_headers.h"
#include "Char.h"
#include "tagexception.h"
namespace Corpus
{

	class CORPUS_API CCharASCII :
		public CChar
	{
	public:
		CCharASCII(unsigned char c) { c_ = c; }
		~CCharASCII(void);

		virtual inline bool IsSpace()
		{
			return isspace(c_) != 0;
		}

		virtual inline bool IsPunct()
		{
			return ispunct(c_) != 0;
		}

		virtual inline void AppendToString(std::string& s)
		{
			s += c_;
		}

		virtual inline bool IsEOF()
		{
			return c_ == (unsigned char)-1;
		}

		virtual inline int Bytes()
		{
			return 1;
		}

		virtual inline unsigned char operator[](int index)
		{
			if(index != 0)
				throw tagexception("Blad indeksowania znaku.");
			return c_;
		}

	private:
		unsigned char c_;
	};

}
#endif

