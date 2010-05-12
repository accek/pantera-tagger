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

#ifndef CORPUSLIB_TOKENISATION_LAYERS_PRETOKENQUEUE_H
#define CORPUSLIB_TOKENISATION_LAYERS_PRETOKENQUEUE_H

#include "../../../corpuslib_headers.h"
#include "../Pretoken.h"


namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			/** A wrapper class for pretoken queue allowing to handle spaces
			  * in a convenient manner. */
			class CORPUS_API CPretokenQueue
			{
			public:
				/// Creates an empty queue.
				CPretokenQueue()
					:nsNow(false)
				{
				}

				/** Resets the space flag. Call before adding parts of a new
				  * pretoken, which used to be surrounded by spaces. */
				void NewWhole(bool withNoSpace = false)
				{
					nsNow = withNoSpace;
				}

				/** Feeds given part of a whole to the queue
				  * (whole == pretoken surrounded by spaces). */
				void PushPart(std::string cp_orth, TFlexClass flex, TCategories attrs);
				
				bool IsEmpty()
				{
					return pretokens.empty();
				}
				
				/// Removes all the pretokens/parts.
				void Clear()
				{
					while(!pretokens.empty())
						pretokens.pop();
					nsNow = false;
				}
				
				/** Removes and returns the first awaiting pretoken/part.
				  * If empty, returns null shared pointer. */
				boost::shared_ptr<SPretoken> Pop();

			private:
				std::queue< boost::shared_ptr<SPretoken> > pretokens;
				bool nsNow;
			};

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus

#endif
