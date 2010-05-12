/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_ATLEASTBINCC_H
#define LIBJOSKIPI_ATLEASTBINCC_H

#include "../joskipi_headers.h"
#include "BinaryComplexC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Operator poszukuj�cy okre�lonej liczby token�w spe�niaj�cych warunek condtions[0], kt�re ma odnale�� pomi�dzy 
		startPos i endPos. W razie, gdyby nie znalaz� zwraca false.
		*/
		class JOSKIPI_API CAtLeastBinCC :
			public CBinaryComplexC
		{
		public:
			/**
			Identyczny jak w klasie bazowej, dodatkowo tylko pobiera ilo�� token�w.
			*/
			CAtLeastBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions, int num);
			virtual ~CAtLeastBinCC(void);

			virtual bool Evaluate(Corpus::CSentence& sentence);

			/**
			\todo Implementacja CAtLeastBinCC::ToString()
			*/
			virtual std::string ToString();

		protected:
			/**
			Minimalna liczba token�w zgodnych z warunkiem, kt�re ma znale�� (mo�e ich by� wi�cej).
			*/
			int num;
		};
	}

}
#endif // LIBJOSKIPI_ATLEASTBINCC_H

