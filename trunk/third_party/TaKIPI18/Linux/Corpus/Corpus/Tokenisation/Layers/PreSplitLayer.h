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

#ifndef CORPUSLIB_TOKENISATION_LAYERS_ALWAYSSPLITLAYER_H
#define CORPUSLIB_TOKENISATION_LAYERS_ALWAYSSPLITLAYER_H

#include "../../../corpuslib_headers.h"
#include "../TokLayer.h"
#include "PretokenQueue.h"


namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			/** The layer responsible for initial splitting of pretokens
			  * according to ALWAYS-SEPARATORS (currently dashes).
			  * It makes possible further splitting into PRE- and
			  * POST-SEPARATORS*/
			class CORPUS_API CPreSplitLayer
				: public CTokLayer
			{
				public:
					CPreSplitLayer(boost::shared_ptr<CTokLayer> lowerLayer)
					:CTokLayer(lowerLayer), queue()
					{
					}

					virtual boost::shared_ptr<SPretoken> GetNext();

				private:
					CPretokenQueue queue;

			};

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus

#endif
