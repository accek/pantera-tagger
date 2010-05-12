/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_CATFLT_H
#define LIBJOSKIPI_CATFLT_H

#include "../joskipi_headers.h"
#include "Operator.h"
#include "ConstValue.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		\todo Opis CCatFlt
		\todo zmieniono values ze statycznego wektora, na wektor dynamiczny
		\todo Napisaæ konstruktor kopiuj¹cy i operator przypisania
		*/
		class JOSKIPI_API CCatFlt :
			public COperator
		{
		public:
			CCatFlt(pos_ptr position, CConstValue* attribs);
			virtual ~CCatFlt(void);

			virtual bool GetValue(std::vector<Corpus::CValue >& val, Corpus::CSentence& sentence);
			virtual void GetValueNames(std::stringstream& out);

			std::string ToString();

		protected:
			CConstValue* attribs;
			// Tablica wyprowadzonych wartoœci (binarnych i stringowych).
			std::vector<Corpus::DLONG>* values;
		};
	}
}
#endif //LIBJOSKIPI_CATFLT_H

