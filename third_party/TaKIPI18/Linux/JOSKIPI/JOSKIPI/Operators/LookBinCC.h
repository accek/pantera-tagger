/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
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
		Abstrakcyjna klasa bazowa dla operator�w poszukuj�cych wyst�pie� okre�lonych token�w (zgodnych z warunkiem). 
		Poniewa� operatory te za ka�dym uruchomieniem mog� zmienia� przekazane im `standardowe' pozycje, konieczne jest 
		jeszcze wyr�nienie w wektorze pozycji pozycji pocz�tkowych--inicjuj�cych. 
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


