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

#ifndef CORPUSLIB_TOKENISATION_LAYERS_INPUTLAYER_H
#define CORPUSLIB_TOKENISATION_LAYERS_INPUTLAYER_H

#include "../../../corpuslib_headers.h"
#include "../TokLayer.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			/** The lowest layer operating on a stream of characters.
			  * It splits the input according to white characters. */
			class CORPUS_API CInputLayer
				: public CTokLayer
			{
			public:
				/** The input stream is assumed to be encoded in
				  * CConfig::inputEncoding. */
				CInputLayer(std::istream &co_inStream);

				~CInputLayer();

				/** Tries to fetch the next pretoken, i.e. a sequence of non-white
				  * characters. The pretoken is converted to the internal encoding
				  * and fed to SPretoken. */
				virtual boost::shared_ptr<SPretoken> GetNext();

			protected:
				/** A work-around before libICU is introduced. We operate
				  * on cp1250 strings, so after splitting with IsBlankUniversal
				  * we need to check against cp1250-specific blanks. */
				void SplitAndPush(const std::string &cp_unsplit);
				
				/// Input stream in external (CConfig) encoding.
				std::istream &co_input;
				
				/** In case current pretoken containts an encoding-specific
				  * blank character, we split it further. */
				std::queue< boost::shared_ptr<SPretoken> > pretokens;
			};

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus

#endif
