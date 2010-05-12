/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_FILTER_H
#define TAKIPI_FILTER_H

#include "../../takipi_headers.h"
#include "../Layers.h"

namespace TaKIPI
{
	namespace Learning
	{
		class TAKIPI_DEV_API CFilter
		{
		public:
			CFilter(void);
			~CFilter(void);

			static void Filter(Corpus::CSentence* in, Corpus::CSentence* out, CLayers& layers, bool previousLayer);
			static bool IsAmb(Corpus::CSentence& sentence, CLayers& layers);
		};

	}//namespace Learning 
}//	namespace TaKIPI

#endif//TAKIPI_FILTER_H

