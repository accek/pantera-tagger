/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
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
		Sprawdza czy w ci�gu token�w wyst�puj� ci�g lexem�w, zgodnych co wzgl�dem N z M podanych atrybut�w (np. przypadku, liczbie).
		*/
		class JOSKIPI_API CAgrBinCC :
			public CBinaryComplexC
		{
		public:
			/*
			Tworzymy warunek podaj�c mu positions (od, do)

			R�ni si� od konstruktora klasy bazowej. Wymagane s� jedynie dwa obiekty typu CPosition,
			dok�adnie: startPos, endPos. Obiekt varPos jest ignorowany!

			Konstruktor ponadto otrzymuje binarnie zakodowany zbi�r atrybut�w w postaci obiektu CConstValue.

			\throw std::string gdy bits < 0
			*/
			CAgrBinCC(std::vector<pos_ptr>* positions, CConstValue* atrSet, int bits) throw (std::string);
			/**
			\todo kto ma zwalniac attributes? Sprawdzic gdzie to jest zwalniane
			*/
			virtual ~CAgrBinCC(void);

			/*
			Metoda zwraca reprezentacj� warunku w JOSKIPI.
			Potrzebna do sprawdzenia czy poprawnie sparsowano warunek.
			*/
			virtual std::string ToString();

		protected:
			/**
			Wska�nik do obiektu CConstValue przechowuj�cego sum� binarn� opisuj�c� zbi�r atrybut�w branych pod uwag� 
			podczas szukania dopasowania leksem�w. 
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
