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

#ifndef TAGEXCEPTION_H_INCLUDED
#define TAGEXCEPTION_H_INCLUDED
#include "../corpuslib_headers.h"

namespace Corpus
{

	class CORPUS_API tagexception : public std::exception
	{
		public:
			tagexception(const char* err_msg) : m_ErrorMsg(err_msg) { }
			virtual ~tagexception() throw() {}
			virtual const char* what() const throw() {return m_ErrorMsg.c_str(); }
		private:
			std::string m_ErrorMsg;

	};
}


#endif
