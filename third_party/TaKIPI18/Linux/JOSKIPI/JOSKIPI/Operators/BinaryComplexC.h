/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKII_BINARYCOMPLEXC_H
#define LIBJOSKII_BINARYCOMPLEXC_H

#include "../joskipi_headers.h"
#include "ComplexCond.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Abstrakcyjna klasa bazowa dla warunk�w z�o�onych pracuj�cych w oparciu o podane (lub ustalane) dwie pozycje
		(st�d nazwa) w zdaniu. 
		*/
		class JOSKIPI_API CBinaryComplexC: public CComplexCond
		{
		public:
			/**
			Wymaga dok�adnie dw�ch warunk�w sk�adowych i dw�ch pozycji.

			Trzy pozycje, na kt�rych pracuj� obiekty tej klasy s� wyr�nione nazwami: startPos, endPos i varPos.
			\throw std::string gdy !positions || (int)positions->size() != 3
			*/
			CBinaryComplexC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions) throw(std::string);
			virtual ~CBinaryComplexC(void);
			
			/// Pomocnicza funkcja, kt�ra mo�e si� przyda� dla innych operator�w.
			static void CalculatePositionLimits(Corpus::CSentence& sentence,
				Corpus::CPosition &s, Corpus::CPosition &e, const Corpus::CPosition &initS, const Corpus::CPosition &initE);

		protected:
			/**
			Dot. startPos i endPos
			Wska�nik lub sta�a wskazuj�ca na pozycj� w tablicy, wyr�nia dwie (jedyne) pozycje na jakich pracuj� obiekty 
			klas pochodnych po danej; warto�ci tych obiekt�w mog� by� zmieniane aby zosta� przekazane do dalszych warunk�w;
			*/
			pos_ptr startPos;
			/**
			Zob. startPos
			*/
			pos_ptr endPos;
			/**
			Obiekt pozycji -- zmienna, po kt�rej b�dziemy iterowa� wewn�trz warunku binarnego (tzn. do kolejnych po�o�e� 
			tej zmiennej b�dzie stosowany sk�adowy warunek wykorzystuj�cy t� zmienn�); stan tej zmiennej (obiektu) b�dzie 
			wykorzystywany do przekazywania znalezionej pozycji na zewn�trz (w wypadku braku znalezionej pozycji stan
			jej to CPosition::nowhere).
			*/
			pos_ptr varPos;

			/**
			Pomocnicza metoda ustalaj�ca dla bie��cego zdania granice dzia�ania operatora binarnego.

			1. if startPos.position == CPosition::beginning st=pocz�tek zdania else st=startPos.position
			2. if endPos.position == CPosition::end end=koniec zdania  else end=startPos.position
			*/
			void CalculateLimits(Corpus::CSentence& sentence, Corpus::CPosition &s, Corpus::CPosition &e);
		};
	}
}
#endif

