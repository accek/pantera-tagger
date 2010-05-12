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

#ifndef CORPUSLIB_XMLWRITER_H
#define CORPUSLIB_XMLWRITER_H

#include "../corpuslib_headers.h"
#include "Sentence.h"

namespace Corpus
{
	typedef std::deque<Corpus::CToken *> TOK_QUE;
	typedef boost::shared_ptr< TOK_QUE > tokque_ptr;
	
	class CORPUS_API CXmlWriter
	{
	public:
		CXmlWriter(std::ostream& xmlFile);
		~CXmlWriter(void);

		/** Writes the sentence. Will resolve segmentation ambiguities, i.e. leave only
		  * one variant according to a primitive heuristics. */
		void WriteSentence(CSentence* sentence, std::string& tags, bool eof);

	private:
		std::ostream& xmlFile_;
		
		/// For remembering the state of pseudo-random number generator.
		unsigned int randstate;
		
		/// For holding ambiguous tokens to resolve later.
		TOK_QUE held;
		
		/// Resolves segmentation ambiguity in held tokens.
		void Resolve();
		
		/** UNUSED YET. A stub currently, prefers variants shorter in tokens.
		  * Should calculate heuristic measure for given variant's rating. */
		double RateVariant(tokque_ptr variant);
		
		/// Taken from POSIX.1-2001 example of rand_r definition (re-entrant pseudo-random generator).
		unsigned int RandomR();
	};

}

#endif

