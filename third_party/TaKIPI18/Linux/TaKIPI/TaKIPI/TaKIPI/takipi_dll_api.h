/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_DLL_API_H
#define TAKIPI_DLL_API_H

__declspec(dllimport) void TaggerStart(void);
__declspec(dllimport) void TaggerEnd(void);
__declspec(dllimport) char *Tag(char const* sentence);
__declspec(dllimport) void SetInputEncoding(int encoding);
__declspec(dllimport) void FreeTaggerMemory(char * buff);
__declspec(dllimport) void SetDataDir(const char *dataPath);

#endif
