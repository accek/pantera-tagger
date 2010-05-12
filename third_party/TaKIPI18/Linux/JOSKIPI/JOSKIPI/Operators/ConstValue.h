/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_CONSTVALUE_H
#define LIBJOSKIPI_CONSTVALUE_H

#include "../joskipi_headers.h"
#include "CondElem.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Reprezentuje ustalon� warto�� w warunkach CCondition . Mo�e to by� pojedyncza, skalarna warto��, 
		lub te� zbi�r warto�ci, st�d w klasie agregacja set	(realizowana jako wektor/tablica).
		Uwaga: je�eli liczno�� set jest 1 to mimo wszystko dany obiekt mo�e	reprezentowa� zbi�r dla warto�ci 
		binarnych, tzn. set[0].bin mo�e by� sum� 1 bitow� warto�ci odpowiednich atrybut�w. 
		Efekt ten b�dzie wykorzystywany w metodach klas pochodnych po CCondition .
		*/
		class JOSKIPI_API CConstValue :	public CCondElem
		{
		public:
			CConstValue(void);
			~CConstValue(void);

			/**
			Dodaje przekazan� warto�� do wektora set.
			*/
			void AddValue(Corpus::CValue p);
			/**
			Doobiektu val klasy ConstValue przekazanego przez referencj� podstawia warto�ci z wektora set. 
			Powinno by�: Wykorzystuje przy tym istniej�ce ju� obiekty w wektorze val.set i w miar� potrzeb dodaje nowe.
			Jest: czy��i wektor val, po czym dodaje wszystkie elementy.
			*/
			bool GetValue(std::vector<Corpus::CValue>& val, Corpus::CSentence& sentence);

			std::string ToString();

		private:
			std::vector<Corpus::CValue>* set;
		};
	}
}
#endif

