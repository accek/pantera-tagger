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

/* morfeusz.h
Copyright (c) by Marcin Woli�ski
$Date: 2004/03/25 11:41:44 $

C language interface for Morfeusz morphological analyser

*/

#ifndef __MORFEUSZ_H__
#define __MORFEUSZ_H__

#include "../../corpuslib_headers.h"
#include "../Parser/Parser.h"
#include "../Token.h"

#ifndef WIN32
#define DLLIMPORT
#else
/* A Windows system.  Need to define DLLIMPORT. */
#if BUILDING_MORFEUSZ
#  define DLLIMPORT __declspec (dllexport)
#else
#  define DLLIMPORT __declspec (dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	/* Returns a string containing information on authors and version of
	the library:
	*/

	DLLIMPORT char *morfeusz_about();



	/*
	The result of analysis is  a directed acyclic graph with numbered
	nodes representing positions  in text (points _between_ segments)
	and edges representing interpretations of segments that span from
	one node to another.  E.g.,

	{0,1,"ja","ja","ppron12:sg:nom:m1.m2.m3.f.n1.n2:pri"}
	|
	|      {1,2,"zosta�","zosta�","praet:sg:m1.m2.m3:perf"}
	|      |
	__|  ____|   __{2,3,"em","by�","aglt:sg:pri:imperf:wok"}
	/  \ /     \ / \
	* Ja * zosta�*em *
	0    1       2   3

	Note that the word 'zosta�em' got broken into 2 separate segments.

	The structure below describes one edge of this DAG:

	*/

	struct _InterpMorf {
		int p, k; /* number of start node and end node */
		char *forma, /* segment (token) */
			*haslo, /* lemma */
			*interp; /* morphosyntactic tag */
	};
	typedef struct _InterpMorf InterpMorf;



	/* Analyse a piece of text:

	'tekst' - the string to be analysed.  It should neither start nor
	end  within a  word.   Morfeusz has  limited  space for  results.
	Don't pass  to this function more  than a typical  paragraph at a
	time.  The best  strategy is probably to pass  to Morfeusz either
	separate words or lines of text.

	RETURNS a  table of  InterpMorf structures representing  edges of
	the  resulting  graph.   The   result  remains  valid  till  next
	invocation of morfeusz_analyse().  The function does not allocate
	any memory, the space is reused on subsequent invocations.

	The  starting node  of resulting  graph has  value of  0  on each
	invocation.  The end of results is marked with a sentinel element
	having the value -1 in the 'p' field.  If a segment is unknown to
	Morfeusz,  the  'haslo'  and  'interp' fields  in  the  resulting
	structure are NULL.
	*/

	DLLIMPORT InterpMorf *morfeusz_analyse(char *tekst);

	/*
	Set options:

	'option' is set to  'value'.  Available options are represented by
	#defines listed below.

	RETURNS 1 (true) on success,  0 (false) on failure (no such option
	or value).
	*/

	DLLIMPORT int morfeusz_set_option(int option, int value);

	/*
	MORFOPT_ENCODING:

	The encoding  used for  'tekst' argument of  morfeusz_analyse and
	fields  'forma',  'haslo',  and  'interp' of  results.   Possible
	values: ISO-8859-2 (default), CP1250.
	*/

#define MORFOPT_ENCODING 1

#define MORFEUSZ_ISO8859_2 88592
#define MORFEUSZ_CP1250 1250
#define MORFEUSZ_UTF8 8

	namespace Corpus
	{
		namespace Morfeusz
		{
			CORPUS_API void Analyse(std::vector<Corpus::CToken *> &out, const std::string &form);
			CORPUS_API void SetEncoding(int encoding);
			
			/** Part of DAG, being a sequence of pmorf edges that corresponds to segmentation ambiguity of one word.
			  * For instance, analysing form miałem will result in one part containing three edges: miał, em, miałem.
			  */
			struct CORPUS_API Part
			{
				int beg, end;
				std::list<InterpMorf *> content;
				
				/// Creates a new part containing given node only.
				Part(InterpMorf *pmorf);
				
				/** Tries to join a part with other.
				Precondition: parts must not be disjoint.
				Postcondition: this part has edges from both parts, that part has no edge and is marked defunct. */
				bool TryJoining(Part &other);
				
				/** Gatheres tokens contained in this part and flushes new CToken objects into given output vector.
				  * Should be called after joining parts. Will resolve segmentation ambiguity and mark tokens'
				  * variant numbers (or set to 0 if no ambiguity). */
				void HarvestTokens(std::vector<Corpus::CToken *> &out);
				
				bool Disjoint(const Part &other) const;
				void Join(Part &other);
				
				void Dump();
			};
			
		}
	}

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* __MORFEUSZ_H__ */
