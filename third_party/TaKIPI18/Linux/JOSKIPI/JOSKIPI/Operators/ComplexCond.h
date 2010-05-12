/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
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
		Abstrakcyjna klasa bazowa dla klas implementuj�cych z�o�one testy logiczne, gdzie kilka warunk�w 
		prostszych jest powi�zanych operatorami logicznymi. 
		*/
		class JOSKIPI_API CComplexCond: public CCondition
		{
		public:
			/**
			Dostaje wektor pozycji w zdaniu (cz�� z nich mo�e by� dopiero `do wype�nienia') i wektor prostszych warunk�w. 
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


