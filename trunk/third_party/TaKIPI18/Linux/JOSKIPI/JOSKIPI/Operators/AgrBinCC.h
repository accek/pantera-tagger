/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_ARGBINCC_H
#define LIBJOSKIPI_ARGBINCC_H

#include "../joskipi_headers.h"
#include "BinaryComplexC.h"
#include "ConstValue.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/*
		Warunek Agreement
		Sprawdza czy w ci¹gu tokenów wystêpujê ci¹g lexemów, zgodnych co wzglêdem N z M podanych atrybutów (np. przypadku, liczbie).
		*/
		class JOSKIPI_API CAgrBinCC :
			public CBinaryComplexC
		{
		public:
			/*
			Tworzymy warunek podaj¹c mu positions (od, do)

			Ró¿ni siê od konstruktora klasy bazowej. Wymagane s¹ jedynie dwa obiekty typu CPosition,
			dok³adnie: startPos, endPos. Obiekt varPos jest ignorowany!

			Konstruktor ponadto otrzymuje binarnie zakodowany zbiór atrybutów w postaci obiektu CConstValue.

			\throw std::string gdy bits < 0
			*/
			CAgrBinCC(std::vector<pos_ptr>* positions, CConstValue* atrSet, int bits) throw (std::string);
			/**
			\todo kto ma zwalniac attributes? Sprawdzic gdzie to jest zwalniane
			*/
			virtual ~CAgrBinCC(void);

			/*
			Metoda zwraca reprezentacjê warunku w JOSKIPI.
			Potrzebna do sprawdzenia czy poprawnie sparsowano warunek.
			*/
			virtual std::string ToString();

		protected:
			/**
			WskaŸnik do obiektu CConstValue przechowuj¹cego sumê binarn¹ opisuj¹c¹ zbiór atrybutów branych pod uwagê 
			podczas szukania dopasowania leksemów. 
			*/
			CConstValue* attributes;


			bool CheckAllForOne(std::vector<std::vector<std::pair<Corpus::CValue,bool> > >& vecTab,
				Corpus::CValue& val);

			virtual bool Evaluate(Corpus::CSentence& sentence);

			int bits;
		};
	}

}
#endif //LIBJOSKIPI_ARGBINCC_H
