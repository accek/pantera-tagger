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

#ifndef CORPUSLIB_GUESSER_API_H
#define CORPUSLIB_GUESSER_API_H

#define GUESSER_UTF8 8
#define GUESSER_WINDOWS1250 1250
#define GUESSER_ISO88592 88592


/* CORPUS_API to deklaracje eksportu do pliku DLL.
 * Ma to znaczenie podczas kompilacji projektu Corpus do biblioteki windowsowej.
 * Jesli ten plik naglowkowy jest wykorzystany jako naglowek interfejsu guessera,
 * robimy zaslepke na CORPUS_API -- pusty ciag (podobnie jak w przypadku
 * kompilacji biblioteki pod linuksem) */
#ifndef CORPUS_API
#define CORPUS_API
#endif


/** Ustawia kodowanie dla guessera, tokenizatora i zewnetrznego sentencera.
  * Jest to globalne kodowanie obowiazujace w projekcie Corpus. Kodowanie to
  * dotyczy danych przesylanych do i z funkcji tego API.
  * Nalezy zachowac ostroznosc przy laczeniu z Tagerem --- jesli korzysta
  * on z tej samej biblioteki ladowanej dynamicznie, globalne kodowanie bedzie
  * zasobem wspoldzielonym (w tagerze mozna osobno ustawiec JEGO kodowanie).
  */
CORPUS_API void SetCorpusEncoding(int encoding);

 /* Ustawia katalog przechowywania danych (do guesser i sentencera).
  * Moze to byc ciag pusty, wtedy wartosc standardowa (prawdopodobnie ../Data). */
CORPUS_API void SetCorpusDataDir(const char *cx_dir);

/** Generuje wynik analizy danej formy. Podana forma powinna byc w miare mozliwosci
  * jednym slowem: jesli morfeusz jest w stanie odgadnac, podzieli prawidlowo
  * na segmenty; jesli nie, odgadywacz potraktuje forme jako jeden segment.
  * Zalecana jest zatem wstepna segmentacja tekstu na podstawie znakow pustych
  * i interpunkcji, po czym przekazywanie segmentow do tej funkcji.
  *
  * Wyniki analizy sa w uproszczonym formacie (przeznaczone glownie do uzycia w Javie):
  * Ka�da analiza (odpowiednik klasy CToken) w nowej linii:
  * ORTH\tBASE1 TAG1\tBASE2 TAG2\t...\tBASEn TAGi\n
  * ORTH -- forma napotkana, BASE -- forma bazowa (lemat),
  * TAG -- tag w formacie KIPI (z dwukropkami)
  *
  * Zwracany ciag znakow nalezy do obszaru pamieci zaalokowanego statycznie,
  * NIE ZWALNIAC GO!
  */
CORPUS_API const char *GuessForm(const char *co_form);

/** Jak powyzej, lecz zwraca analize w postaci XML-owej w formacie KIPI.
  * Zwracany XML nie ma korzenia, zawiera ci�g elementow <tok>...</tok>.
  * Co ze zwalnianiem pamieci?
  */
CORPUS_API const char *GuessFormXml(const char *co_form);

/** Dzieli tekst na segmenty KIPI. W przypadku rozpoznania formy wyrazowej
  * nie jest ona poddawana analizie morfologicznej --- pozostaje klasa twf. */
CORPUS_API const char *TokeniseInputXml(const char *co_input);

/** Zewnetrzny modul sentencera, byc moze zostanie wycofany z dalszych wersji
  * Corpus, poki co opisu brak. */
CORPUS_API int SentencerFeed(const char *orth, const bool wasNS, const bool isSym);
CORPUS_API int SentencerFed();
CORPUS_API void SentencerReset();

#endif
