/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		S³abe uzgodnienie.
		Sprawdza czy w ci¹gu tokenów wystêpujê ci¹g leksemów zgodnych co do wartoœci podanych atrybutów.
		Wystarczy, jeœli tokeny uzgodnione s¹ na wartoœciach tych atrybutów, które posiada.
		*/
		class JOSKIPI_API CAgrWeakBinCC :
			public CBinaryComplexC
		{
		public:
			/*
			Tworzymy warunek podaj¹c mu positions (od, do)

			Ró¿ni siê od konstruktora klasy bazowej. Wymagane s¹ jedynie dwa obiekty typu CPosition,
			dok³adnie: startPos, endPos. Obiekt varPos jest ignorowany!

			Konstruktor ponadto otrzymuje binarnie zakodowany zbiór atrybutów w postaci obiektu CConstValue.
			*/
			CAgrWeakBinCC(std::vector<pos_ptr>* positions, CConstValue* atrSet, int bits) throw (std::string);
			/**
			\todo kto ma zwalniac attributes? Sprawdzic gdzie to jest zwalniane
			*/
			virtual ~CAgrWeakBinCC(void);

			/*
			Metoda zwraca reprezentacjê warunku w JOSKIPI.
			Potrzebna do sprawdzenia czy poprawnie sparsowano warunek.
			*/
			virtual std::string ToString();
			
			/// Funkcja jest te¿ u¿ywana przez filtr uzgodnienia, dlatego jest statyczna i publiczna.
			static bool FindAgrPaths(Corpus::CSentence &sentence, std::vector< std::set< Corpus::CValue > > &paths,
				std::vector< Corpus::CValue > &mask, int s_pos, int e_pos, int min_bits);
			

		protected:
			/**
			WskaŸnik do obiektu CConstValue przechowuj¹cego sumê binarn¹ opisuj¹c¹ zbiór atrybutów branych pod uwagê 
			podczas szukania dopasowania leksemów. 
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
