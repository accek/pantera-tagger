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

#ifndef CORPUSLIB_CHARUTF_H
#define CORPUSLIB_CHARUTF_H

#include "../corpuslib_headers.h"

#include "Char.h"
#include "tagexception.h"

namespace Corpus
{
	class CORPUS_API CCharUTF :
		public CChar
	{
	public:
		CCharUTF(std::vector<unsigned char> utfChars);
		~CCharUTF(void);

		virtual bool IsSpace();
		virtual bool IsPunct();
		virtual bool IsEOF();
		virtual void AppendToString(std::string& s);
		virtual unsigned char operator[](int index);
		virtual inline int Bytes() { return (int)utfChars_.size(); }

	private:
#ifdef WIN32
		template class CORPUS_API std::allocator<unsigned char>;
		template class CORPUS_API std::vector<unsigned char, std::allocator<unsigned char> >;
#endif
		std::vector<unsigned char> utfChars_;
	};
}
#endif

