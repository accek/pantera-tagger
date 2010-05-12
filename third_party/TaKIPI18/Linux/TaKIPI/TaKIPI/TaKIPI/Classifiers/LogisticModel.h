/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_LOGISTICMODEL_H
#define TAKIPI_LOGISTICMODEL_H

#include "../../takipi_headers.h"
#include "../Layers.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		class TAKIPI_DEV_API CLogisticModel
		{
		public:
			CLogisticModel(void);
			CLogisticModel(std::vector<std::string> modelData);
			void Parse(std::vector<std::string> modelData);
			~CLogisticModel(void);
			std::string ToString();
			std::map<Corpus::CValue, double> GetLogs(JOSKIPI::Patterns::CPattern &pattern, Corpus::CSentence & sentence);
		private:
			// model = klasa -> <wyraz_wolny; atrybut -> wartosc_atrybutu -> wspolczynnik>
			std::map< Corpus::CValue, std::pair< double, std::map< int, std::map< Corpus::DLONG, double> * > > > *model;

			CLogisticModel(const CLogisticModel&) ; //no implementation
			CLogisticModel& operator=(CLogisticModel&); //no implementation
		};
	}// namespace Classifiers
}//namespace TaKIPI

#endif//TAKIPI_LOGISTICMODEL_H

