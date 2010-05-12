/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Adam Radziszewski, Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_AGRFLT_H
#define LIBJOSKIPI_AGRFLT_H

#include "../joskipi_headers.h"
#include "Operator.h"
#include "ConstValue.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/** Agreement filter. Tries to find weak agreement between given positions and, if succeeded, returns values
		  * of fltAttribs that satisfy the agreement. NOTE: ftrAttrs must be a subset of agrAttrs. 
		  *
		  * For example:
		  *   agrflt(-2, 0, {cas,nmb,gnd}, 3, {cas})
		  *
		  * will try to find weak agreement on three tokens (at -2.-1,0) and return case values that satisfy the agreement.
		  */
		class JOSKIPI_API CAgrFlt :
				public COperator
		{
			public:
				CAgrFlt(pos_ptr startPos, pos_ptr endPos, CConstValue *agrAttrs, int agrBits, CConstValue *fltAttrs) throw (std::string);
				virtual ~CAgrFlt(void);

				virtual bool GetValue(std::vector<Corpus::CValue > &val, Corpus::CSentence& sentence);
				virtual void GetValueNames(std::stringstream &out);

				std::string ToString();

			protected:
				pos_ptr endPos;
				int bits;
				CConstValue *agrAttribs, *fltAttribs;
				// output values
				std::vector<Corpus::DLONG> *values;
				
				Corpus::CValue MaskedValues(std::set<Corpus::CValue> &where, const Corpus::CValue &mask);
		};
	}
}

#endif //LIBJOSKIPI_AGRFLT_H
