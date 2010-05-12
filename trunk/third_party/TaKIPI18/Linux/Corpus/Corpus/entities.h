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

#ifndef CORPUSLIB_TOKENISATION_XMLUTILS_ENTITIES_H
#define CORPUSLIB_TOKENISATION_XMLUTILS_ENTITIES_H

#include "../corpuslib_headers.h"
namespace Corpus
{
	/** Decodes XML entities in given string (there are five of them,
		* e.g. &quot;). The string is overwritten. The cx_ prefix means
		* that any character encoding should be applicable (assuming that
		* lower half of each byte is conformant with ASCII). */
	CORPUS_API void DecodeEntities(std::string &cx_input);
	
	/** Encodes characters unwelcome in XML data (there are five of
		* them) into XML entity representation. The string is
		* overwritten. */
	CORPUS_API void EncodeEntities(std::string &cx_input);
} // namespace Corpus

#endif
