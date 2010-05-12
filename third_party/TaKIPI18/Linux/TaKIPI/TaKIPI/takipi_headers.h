/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_TAKIPI_HEADERS_H
#define TAKIPI_TAKIPI_HEADERS_H

//#define TAKIPI_DEVELOPER

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
	// Windows Header Files:
	#include <windows.h>

/**
TAKIPI_API -> dla uzytkownika TaKIPI
TAKIPI_DEVELOPER -> dla programistow rozwijajacych TaKIPI
*/

	// DLL signature:
	#ifdef takipi_EXPORTS
	#define TAKIPI_EXPORTS
	#endif
	
	#ifdef TAKIPI_EXPORTS
	#define TAKIPI_API __declspec(dllexport)
	#else
	#define TAKIPI_API   __declspec(dllimport)
	#endif
	
#ifdef TAKIPI_DEVELOPER
#define TAKIPI_DEV_API TAKIPI_API
#else
#define TAKIPI_DEV_API	
#endif

#else // nie WIN32
	#define TAKIPI_API
	#define TAKIPI_DEV_API
#include <sys/stat.h> // do mkdir
#include <sys/types.h> // do mkdir
#include <dirent.h> // do IO

#endif //WIN32

#include <Corpus/corpuslib_headers.h>
#include <Corpus/Corpus/corpus_all.h>
#include <JOSKIPI/joskipi_headers.h>
#include <JOSKIPI/all_joskipi.h>

#include "math.h"


#endif //TAKIPI_TAKIPI_HEADERS_H
