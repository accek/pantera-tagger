/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_LOOKBINCC_H
#define LIBJOSKIPI_LOOKBINCC_H

#include "../joskipi_headers.h"
#include "BinaryComplexC.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Abstrakcyjna klasa bazowa dla operatorów poszukuj¹cych wyst¹pieñ okreœlonych tokenów (zgodnych z warunkiem). 
		Poniewa¿ operatory te za ka¿dym uruchomieniem mog¹ zmieniaæ przekazane im `standardowe' pozycje, konieczne jest 
		jeszcze wyró¿nienie w wektorze pozycji pozycji pocz¹tkowych--inicjuj¹cych. 
		\todo uproscic look
		*/
		class JOSKIPI_API CLookBinCC: public CBinaryComplexC
		{
		public:
			CLookBinCC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions);
			virtual ~CLookBinCC(void);

		protected:
			Corpus::CPosition iniStartPos;
			Corpus::CPosition iniEndPos;
		};
	}
}
#endif


