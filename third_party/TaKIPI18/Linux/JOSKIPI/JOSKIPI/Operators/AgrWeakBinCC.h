/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_ARGWEAKBINCC_H
#define LIBJOSKIPI_ARGWEAKBINCC_H

#include "../joskipi_headers.h"
#include "BinaryComplexC.h"
#include "ConstValue.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		S�abe uzgodnienie.
		Sprawdza czy w ci�gu token�w wyst�puj� ci�g leksem�w zgodnych co do warto�ci podanych atrybut�w.
		Wystarczy, je�li tokeny uzgodnione s� na warto�ciach tych atrybut�w, kt�re posiada.
		*/
		class JOSKIPI_API CAgrWeakBinCC :
			public CBinaryComplexC
		{
		public:
			/*
			Tworzymy warunek podaj�c mu positions (od, do)

			R�ni si� od konstruktora klasy bazowej. Wymagane s� jedynie dwa obiekty typu CPosition,
			dok�adnie: startPos, endPos. Obiekt varPos jest ignorowany!

			Konstruktor ponadto otrzymuje binarnie zakodowany zbi�r atrybut�w w postaci obiektu CConstValue.
			*/
			CAgrWeakBinCC(std::vector<pos_ptr>* positions, CConstValue* atrSet, int bits) throw (std::string);
			/**
			\todo kto ma zwalniac attributes? Sprawdzic gdzie to jest zwalniane
			*/
			virtual ~CAgrWeakBinCC(void);

			/*
			Metoda zwraca reprezentacj� warunku w JOSKIPI.
			Potrzebna do sprawdzenia czy poprawnie sparsowano warunek.
			*/
			virtual std::string ToString();
			
			/// Funkcja jest te� u�ywana przez filtr uzgodnienia, dlatego jest statyczna i publiczna.
			static bool FindAgrPaths(Corpus::CSentence &sentence, std::vector< std::set< Corpus::CValue > > &paths,
				std::vector< Corpus::CValue > &mask, int s_pos, int e_pos, int min_bits);
			

		protected:
			/**
			Wska�nik do obiektu CConstValue przechowuj�cego sum� binarn� opisuj�c� zbi�r atrybut�w branych pod uwag� 
			podczas szukania dopasowania leksem�w. 
			 */
			CConstValue* attributes;
			int bits;
			
			virtual bool Evaluate(Corpus::CSentence& sentence);
			static bool ValuesInter(const Corpus::CValue &first, const Corpus::CValue &second);
			static void Constrain(std::set<Corpus::CValue> &what, std::set<Corpus::CValue> &constr);
		};
	}

}
#endif //LIBJOSKIPI_ARGWEAKBINCC_H
