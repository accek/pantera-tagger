/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_CUTOFF_H
#define TAKIPI_CUTOFF_H

#include "../takipi_headers.h"
#include "Layers.h"

namespace TaKIPI
{
	class TAKIPI_DEV_API CCutOff
	{
	public:
		CCutOff(double level);
		~CCutOff(void);

		void CutOff(Corpus::CSentence& sentence);
		void CutWorstPackage(Corpus::CSentence& sentence, TaKIPI::CLayers& layers);
		void LeaveBestPackage(Corpus::CSentence& sentence, TaKIPI::CLayers& layers);

		static long counter;

	private:
		double cutOffLevel;
	};

}//namespace TaKIPI

#endif // TAKIPI_CUTOFF_H


