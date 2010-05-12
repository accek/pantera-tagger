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

#ifndef CORPUSLIB_TOKENISATION_TOKLAYER_H
#define CORPUSLIB_TOKENISATION_TOKLAYER_H

#include "../../corpuslib_headers.h"
#include "Pretoken.h"
#include "../ResourceManager.h"
#include "Core.h"

namespace Corpus
{
	namespace Tokenisation
	{
		/** An abstract base for tokeniser's processing layers. It receives
		  * pretokens from lower layers (or input stream in case of the lowest
		  * layer) and returns processed pretokens. There are no constraints
		  * on the number of output pretokens (input pretokens may be either
		  * split up or joined together). It is required that a NULL pretoken
		  * pointer be output when no more pretokens are to be returned. */
		class CORPUS_API CTokLayer
		{
		public:
			/// Constructs a new layer using given lower layer to operate on.
			CTokLayer(boost::shared_ptr<CTokLayer> lowerLayer);

			/// Constructs a new layer with no lower layer.
			CTokLayer();

			/// The destructor.
			virtual ~CTokLayer();

			/** Returns a pointer to the incoming pretoken or a NULL pointer
			  * if there is nothing left. */
			virtual boost::shared_ptr<SPretoken> GetNext() = 0;

			/// Returns the shared tokeniser core.
			CCore *GetCore()
			{
				return Corpus::CResourceManager::GetInstance()->GetTokeniserCore();
			}

		protected:
			boost::shared_ptr<CTokLayer> lower;
		};

	} // namespace Tokenisation
} // namespace Corpus

#endif
