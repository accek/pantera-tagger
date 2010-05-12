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

#ifndef CORPUSLIB_TOKENISATION_XMLUTILS_FRAGMENTREADER_H
#define CORPUSLIB_TOKENISATION_XMLUTILS_FRAGMENTREADER_H

#include "../../../corpuslib_headers.h"
namespace Corpus
{
	namespace Tokenisation
	{
		namespace XmlUtils
		{
			/** A sort of XML preprocessor. Splits an XML file into so called
			  * fragments, i.e. text strings being either contents of XML tags
			  * or PCDATA (character data between tags). Fragments are
			  * retrieved by subsequent NextFragment calls. Note that whenever
			  * a tag follows another tag, there will be an empty PCDATA (plain
			  * text) tag.
			  * Fragment reader is merely a preprocessor; by no means should it
			  * be used as a final reader. */
			class CORPUS_API CFragmentReader
			{
			public:
				const static char TAG_OPENING;
				const static char TAG_CLOSING;
				
				/// Describes the type of the fragment read.
				enum FragType
				{
				/// Plain text between tags (PCDATA).
					plain,
				/// Tag string (stored without surrounding angle brackets).
					tag
				};
				
				/// Last fragment string.
				std::string frag;
				
				/// Last fragment type.
				FragType type;
				
				/** Creates a fragment reader with given input stream.
				  * The input encoding actually does not matter as long as
				  * angle brackets are encoded as in ASCII (CP1250, CP852,
				  * ISO8859-2 as well as UTF-8 are safe). The encoding will be
				  * left unchanged in output fragments. */
				CFragmentReader(std::istream &co_inputStream);
				
				~CFragmentReader();
				
				/** Tries to fetch the next fragment into "frag" field and its
				  * type into "type" field. Returns if succeeded (false means
				  * there are no more fragments). Should be called before
				  * accessing the fragment stored in class' fields. */
				bool NextFragment(bool decodeEntities = true);
				
				/// Dumps debugging info into stdout.
				void Dump();
				
			private:
				std::istream &co_input;
				
				/** When plain we expect an opening tag; otherwise we expect
				  * a closing tag. */
				FragType state;
			};
			
		} // namespace XmlUtils
	} // namespace Tokenisation
} // namespace Corpus

#endif
