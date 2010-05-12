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

#ifndef CORPUSLIB_TOKENISATION_PREMORPHREADER_H
#define CORPUSLIB_TOKENISATION_PREMORPHREADER_H

#include "../../corpuslib_headers.h"
#include "TokenReader.h"
#include "../Token.h"
#include "XmlUtils/FragmentReader.h"
#include "Tokeniser.h"

namespace Corpus
{
	namespace Tokenisation
	{
		/** A special reader for extracting characted data between tags
		  * (PCDATA) from XML files, especially pre_morph.xml files. Extracted
		  * PCDATAs are put through a tokeniser. Its output is available via
		  * GetNext calls. Tokeniser parameters can be specified in the
		  * constructor.
		  *
		  * Basically, each PCDATA is treated as a separate fragment ("empty
		  * PCDATAs" are omitted, but at least one character between tags will
		  * make the premorph reader see a fragment; thus in some cases some
		  * post-processing might be needed).
		  *
		  * Before getting tokens use NextFragment to fetch the fragment and
		  * determine if it is there. Then fetch tokens of current fragment
		  * util GetNext returns NULL. It can be described with the following
		  * schema:
		  * while NextFragment()
		  *    (while GetNext returns not-NULL
		  *        (process the token; free the token if applicable)).
		  *
		  * You can specify an output stream (as co_tagOutStream) to which XML
		  * tags should be flushed. It is useful for creating morph.xml file,
		  * which contains unchanged XML tags surrounding processed tokens.
		  * If input XML tags are not important pass NULL pointer as
		  * co_tagOutStream.
		  *
		  * By default, output tokens are encoded in CP1250 (this is the
		  * default tokeniser behaviour). Reencoding can be turned on so that
		  * the output tokens are encoded in Corpus::CConfig::inputEncoding.
		  * This does not influence tags which are ALWAYS output in unchanged
		  * encoding (i.e. always in Corpus::CConfig::inputEncoding).
		  */
		class CORPUS_API CPremorphReader: public CTokenReader
		{
		public:
			/** Creates a PRE_MORPH reader with given input stream (XML data)
			  * and given tag output, i.e. a stream to which XML tags will be
			  * flushed. This way XML PCDATAs are extracted for processing
			  * while tags are left unchanged and flushed to the output.
			  *
			  * If tags are not important a NULL pointer can be passed as
			  * co_tagOutStream (then tags will be discarded).
			  *
			  * Parameters useMorpher, reencode and noNewTags are passed
			  * directly to CTokeniser constructor. By default, CPremorph
			  * reader produces full tokeniser output encoded in CP1250.
			  */
			CPremorphReader(std::istream &co_inStream, std::ostream *co_tagOutStream,
							bool useMorpher = true,
							bool reencode = false, bool noNewTags = false);
			
			~CPremorphReader();
			
			CToken *GetNext();
			
			/** Fetches the next fragment. Returns false if there are no more
			  * fragments. Call it before trying to fetch tokens (GetNext). */
			bool NextFragment();

		private:
			std::ostream *co_tagoutput;
			XmlUtils::CFragmentReader fragReader;
			bool tokrUseMorpher, tokrReencode, tokrNoNewTags;
			std::stringstream fragBuffer;
			CTokeniser *fragTokr;
			
			bool GetPlainFragment();
		};
	} // namespace Tokenisation
} // namespace Corpus

#endif
