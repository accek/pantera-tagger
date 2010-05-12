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

#ifndef CORPUSLIB_PLAINWRITER_H
#define CORPUSLIB_PLAINWRITER_H

#include "Token.h"
#include "Tokenisation/Tokeniser.h"

namespace Corpus
{
	/** Klasa pozwalajaca na zapisywanie tagow w uproszczonej postaci tekstowej
	  * (nie XML) oraz w prostej postaci XML (sklejenie XML-i z poszczegolnych
	  * tokenow). Napisana zostala z mysla o laczeniu kodu javowego z ta biblioteka.
	  * Do innych celow zalecane jest raczej uzywanie bezposrednio struktur
	  * wygenerowanych przez FormsIdentifier (CToken, CLexem, CTag itp.), jako
	  * ze nie wymagaja one parsowania.
	  */
	class CORPUS_API PlainWriter
	{
	public:
		/** Zwraca tekst opisujacy tag w formacie:
		  * ORTH\tBASE1 TAG1\tBASE2 TAG2\t...\tBASEn TAGi\n
		  * (kazda taka linia opisuje jeden token)
		  * Uwaga: zwracany ciag znakow nalezy do obszaru pamieci alokowanego
		  * statycznie, NIE ZWALNIAC GO!
		  */
		static const char *GeneratePlainTagString(std::vector<Corpus::CToken *> &tokens);

		/** Zwraca tekst opisujacy tag w formacie XML.
		  * XML tworzony jest poprzez sklejenie opisu XML-owego kolejnych tagow,
		  * dlatego tez w ogolnosci nie ma korzenia
		  * (jest ciagiem elementow <tok>...</tok>
		  */
		static const char *GenerateXmlTagString(std::vector<Corpus::CToken *> &tokens);

		/** Zwraca tekst opisujacy wyjscie z tokenizatora w formacie XML.
		  * Zwracany XML nie ma korzenia. */
		static const char *GenerateXmlTagString(Tokenisation::CTokeniser &tokr);
	};

}

#endif
