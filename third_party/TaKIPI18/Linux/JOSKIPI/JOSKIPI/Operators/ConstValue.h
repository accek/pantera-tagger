/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		Reprezentuje ustalon¹ wartoœæ w warunkach CCondition . Mo¿e to byæ pojedyncza, skalarna wartoœæ, 
		lub te¿ zbiór wartoœci, st¹d w klasie agregacja set	(realizowana jako wektor/tablica).
		Uwaga: je¿eli licznoœæ set jest 1 to mimo wszystko dany obiekt mo¿e	reprezentowaæ zbiór dla wartoœci 
		binarnych, tzn. set[0].bin mo¿e byæ sum¹ 1 bitow¹ wartoœci odpowiednich atrybutów. 
		Efekt ten bêdzie wykorzystywany w metodach klas pochodnych po CCondition .
		*/
		class JOSKIPI_API CConstValue :	public CCondElem
		{
		public:
			CConstValue(void);
			~CConstValue(void);

			/**
			Dodaje przekazan¹ wartoœæ do wektora set.
			*/
			void AddValue(Corpus::CValue p);
			/**
			Doobiektu val klasy ConstValue przekazanego przez referencjê podstawia wartoœci z wektora set. 
			Powinno byæ: Wykorzystuje przy tym istniej¹ce ju¿ obiekty w wektorze val.set i w miarê potrzeb dodaje nowe.
			Jest: czyœæi wektor val, po czym dodaje wszystkie elementy.
			*/
			bool GetValue(std::vector<Corpus::CValue>& val, Corpus::CSentence& sentence);

			std::string ToString();

		private:
			std::vector<Corpus::CValue>* set;
		};
	}
}
#endif

