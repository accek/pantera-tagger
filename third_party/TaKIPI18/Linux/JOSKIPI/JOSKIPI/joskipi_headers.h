/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_JOSKIPI_HEADERS_H
#define LIBJOSKIPI_JOSKIPI_HEADERS_H

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

	#ifdef joskipi_EXPORTS
	#define JOSKIPI_EXPORTS
	#endif
	
	#ifdef JOSKIPI_EXPORTS
	#define JOSKIPI_API __declspec(dllexport)
	#else
		#ifdef JOSKIPI_STATIC
		#define JOSKIPI_API  
		#else
		#define JOSKIPI_API  __declspec(dllimport)
		#endif
	#endif

#else 
	#define JOSKIPI_API
#endif //WIN32

#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include "Corpus/corpuslib_headers.h"
#include "Corpus/Corpus/corpus_all.h"

typedef boost::shared_ptr<Corpus::CPosition> pos_ptr;

#endif

