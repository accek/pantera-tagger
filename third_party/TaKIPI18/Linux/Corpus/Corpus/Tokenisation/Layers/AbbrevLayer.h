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

#ifndef CORPUSLIB_TOKENISATION_LAYERS_ABBREVLAYER_H
#define CORPUSLIB_TOKENISATION_LAYERS_ABBREVLAYER_H

#include "../../../corpuslib_headers.h"
#include "../TokLayer.h"
#include "PretokenQueue.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			/** A layer for fixing broken multiword abbreviations.
			 * Symbols representing multiword abbreviation are joined with
			 * their last dots, e.g. "m.in" with "." (it is split so by the
			 * Split layer).
			 */
			class CORPUS_API CAbbrevLayer
				: public CTokLayer
			{
				public:
					CAbbrevLayer(boost::shared_ptr<CTokLayer> lowerLayer)
					:CTokLayer(lowerLayer)
					{
					}
					virtual boost::shared_ptr<SPretoken> GetNext();

				private:
				// in case an abbrev needs to be joined with its dot token
					std::queue< boost::shared_ptr<SPretoken> > queue;
			};

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus

#endif
