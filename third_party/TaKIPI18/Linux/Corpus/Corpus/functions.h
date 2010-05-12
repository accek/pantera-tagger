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

#ifndef CORPUSLIB_FUNCTIONS_H
#define CORPUSLIB_FUNCTIONS_H

#include "../corpuslib_headers.h"

#ifdef LINUX

char* itoa(int value, char* str, int radix);

char* ultoa(unsigned long value, char* str, int radix);

char *lltoa(unsigned long long value, char* buffer, int radix);

char *ulltoa(long long value, char* buffer, int radix);

char* _i64toa(int value, char* bufor, int radix);

int strnicmp(const char *string1, const char *string2, size_t size);

int stricmp(const char *string1, const char *string2);

#endif


#endif

