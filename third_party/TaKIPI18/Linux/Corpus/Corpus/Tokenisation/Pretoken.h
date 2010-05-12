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

#ifndef CORPUSLIB_TOKENISATION_PRETOKEN_H
#define CORPUSLIB_TOKENISATION_PRETOKEN_H

#include "../../corpuslib_headers.h"
#include "../Tag.h"

namespace Corpus
{
	namespace Tokenisation
	{
		/** A structure representing token in early stage, i.e. without
		  * morphological details. Pretokens can have flex classes
		  * and attributes' values specified, yet this is optional.
		  */
		struct CORPUS_API SPretoken
		{
			/// Form as occured in running text.
			std::string cp_orth;

			/// True iff no space came before this pretoken.
			bool wasNS;

			/** Flex class (POS) of this pretoken. cf_none == not set,
			  * though cf_ign is preferred (not disambiguated yet). */
			TFlexClass flexClass;
			
			/** The encoded values of attributes of current grammatical
			  * category. gc_none == none set. */
			TCategories attrValues;

			/** Creates a new pretoken with given form, preceding space flag
			  * and flex class == cf_ign. */
			SPretoken(const std::string &cp_newOrth, bool noSpace)
				:cp_orth(cp_newOrth), wasNS(noSpace),
			flexClass(cf_ign), attrValues(gc_none)
			{
			}

			void Dump()
			{
				if(wasNS) std::cout << "NS ";
				std::cout << "orth=[" << cp_orth << "]\n";
			}
		};

	} // namespace Tokenisation
} // namespace Corpus

#endif
