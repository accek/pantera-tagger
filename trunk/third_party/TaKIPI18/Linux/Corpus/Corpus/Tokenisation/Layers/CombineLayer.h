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

#ifndef CORPUSLIB_TOKENISATION_LAYERS_COMBINELAYER_H
#define CORPUSLIB_TOKENISATION_LAYERS_COMBINELAYER_H

#include "../../../corpuslib_headers.h"
#include "../TokLayer.h"
#include "PretokenQueue.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			/** A layer designed for the final processing stage, i.e. combining
			  * multiple tokens into bigger ones. Current implementation uses
			  * it for joining multiple minus signs together as they are probably
			  * used to represent em-/en-dash.
			  */
			class CORPUS_API CCombineLayer
				: public CTokLayer
			{
			public:
				CCombineLayer(boost::shared_ptr<CTokLayer> lowerLayer)
					:CTokLayer(lowerLayer)
				{
				}
				virtual boost::shared_ptr<SPretoken> GetNext();

			private:
				std::queue< boost::shared_ptr<SPretoken> > queue;
				
				boost::shared_ptr<SPretoken> Fetch();
			};

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus

#endif
