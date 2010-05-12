/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_COMPLEXCONDITION_H
#define LIBJOSKIPI_COMPLEXCONDITION_H

#include "../joskipi_headers.h"
#include "Condition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Abstrakcyjna klasa bazowa dla klas implementuj¹cych z³o¿one testy logiczne, gdzie kilka warunków 
		prostszych jest powi¹zanych operatorami logicznymi. 
		*/
		class JOSKIPI_API CComplexCond: public CCondition
		{
		public:
			/**
			Dostaje wektor pozycji w zdaniu (czêœæ z nich mo¿e byæ dopiero `do wype³nienia') i wektor prostszych warunków. 
			*/
			CComplexCond(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions);
			/*
			Niszczy tylko obiekty z conditions.
			*/
			virtual ~CComplexCond(void);

		protected:
			std::vector<CCondition*>* conditions;
		};
	}
}
#endif //LIBJOSKIPI_COMPLEXCONDITION_H


