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

#ifndef CORPUSLIB_TOKENISATION_LAYERS_CLASSIFYLAYER_H
#define CORPUSLIB_TOKENISATION_LAYERS_CLASSIFYLAYER_H

#include "../../../corpuslib_headers.h"
#include "../TokLayer.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			/** The main layer responsible for classifying already split pretokens.
			  * The classification rules have been moved to the tokeniser core (Core.h)
			  * as they use regexes, which should be loaded once for the lifetime
			  * of the library. */
			class CORPUS_API CClassifyLayer
				: public CTokLayer
			{
			public:
				CClassifyLayer(boost::shared_ptr<CTokLayer> lowerLayer)
					:CTokLayer(lowerLayer)
				{
					
				}

				virtual boost::shared_ptr<SPretoken> GetNext();
			};

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus

#endif
