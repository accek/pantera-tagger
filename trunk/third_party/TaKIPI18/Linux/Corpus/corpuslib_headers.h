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

#ifndef CORPUS_HEADERS_H
#define CORPUS_HEADERS_H

#ifndef LINUX // WINDOWS
#pragma once
	#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
	#include <windows.h>
	#include <direct.h>
	#include <io.h>
	#include <tchar.h>

	// Library signature:
	#ifdef corpus_EXPORTS
	#define CORPUS_EXPORTS
	#endif
	
	#ifdef CORPUS_EXPORTS
	#define CORPUS_API __declspec(dllexport)
	#else
		#ifdef CORPUS_STATIC
		#define CORPUS_API  
		#else
		#define CORPUS_API  __declspec(dllimport)
		#endif
	#endif

	#pragma warning( disable: 4290 ) //warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)	
	#pragma warning( disable: 4996 ) // c code "unsafe" :/

	#pragma warning(disable:4251) // "need to have dll interface" for stl  // TODO



#else // LINUX
	#define CORPUS_API
	#include <iconv.h>
#endif

#include <time.h>
#include <errno.h>
// standard stl headers
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <queue>
#include <set>
#include <stack>

// boost headers
#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>

// ICU headers
#include <unicode/ustring.h>

#endif
