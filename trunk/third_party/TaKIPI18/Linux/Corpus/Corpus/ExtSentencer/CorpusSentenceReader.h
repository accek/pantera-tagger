/*  Biblioteka Corpus
 *  Zestaw narz�dzi do przetwarzania tekst�w w j�zyku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc�awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardy�ski
 *
 *  Kod na licencji GPL.
 */

#ifndef CORPUSLIB_EXTSENTENCER_CORPUSSENTENCEREADER_H
#define CORPUSLIB_EXTSENTENCER_CORPUSSENTENCEREADER_H

#include "../../corpuslib_headers.h"
#include "SentenceReader.h"
#include "../Parser/Parser.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		/** Corpus sentence reader should be used to read KIPI morph.xml files
		  * when division into sentences is relevant. Note that input morph.xml
		  * file must contain sentence chunks.
		  *.
		  * Remember to dispose of output sentence after use by calling
		  * CSentenceReader::Free. */
		class CORPUS_API CCorpusSentenceReader: public CSentenceReader
		{
		public:
			/** Creates a corpus sentence reader. Input tokens are assumed to be
			  * in Corpus::CConfig::inputEncoding. The default behaviour is to
			  * encode output tokens in CP1250. This can be altered by passing
			  * convertToCP = false (then outputEnc == inputEnc).
			  * When onlySentChunks set to false, the reader will treat any
			  * chunk as sentence chunk, regardless of its type.
			  * Set chunkOutput to an output stream to save non-token XML material.
			  */
			CCorpusSentenceReader(std::istream &co_inStream, bool disambOnly = false,
					bool convertToCP = true, bool onlySentChunks = true, std::ostream *chunkOutput = NULL);
			~CCorpusSentenceReader();

			/** Fetches next sentence from input corpus. The option
			  * eliminateSubstGerIgn pre-processes ambiguities:
			  * tokens with subst and ger lexems are replaced with both subst and ger in one tag;
			  * tokens containing ign tag are replaced with ign-only token */
			CSentence *GetNext(bool eliminateSubstGerIgn = false);

		private:
			std::istream &co_inputStream;
			bool filterDisamb, convertOutput;
			std::string chunkBeg;
			std::ostream *chunkOut;
			std::string unflushedLine;
			
			// std::string GetNextTokenText(std::istream &co_inStream);
		};
	} // namespace ExtSentencer
} // namespace Corpus

#endif
