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

#ifndef CORPUSLIB_TOKENISATION_LAYERS_SPLITLAYER_H
#define CORPUSLIB_TOKENISATION_LAYERS_SPLITLAYER_H

#include "../../../corpuslib_headers.h"
#include "../TokLayer.h"
#include "PretokenQueue.h"


namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			/** The layer responsible for splitting pretokens. They are
			  * divided into a sequence of pre-separators, the body and
			  * post-separators. */
			class CORPUS_API CSplitLayer
				: public CTokLayer
			{
				public:
					CSplitLayer(boost::shared_ptr<CTokLayer> lowerLayer)
					:CTokLayer(lowerLayer), queue()
					{
					}

					virtual boost::shared_ptr<SPretoken> GetNext();

				private:
					CPretokenQueue queue;

					void FindBodyBounds(const char *cp_orth, int &bodyBeg, int &bodyEnd);
					bool IsBeforeBody(const char ch);
					bool IsAfterBody(const char ch);

			};

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus

#endif
