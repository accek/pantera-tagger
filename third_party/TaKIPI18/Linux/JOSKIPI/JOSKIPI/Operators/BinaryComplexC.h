/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
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
		Abstrakcyjna klasa bazowa dla warunków z³o¿onych pracuj¹cych w oparciu o podane (lub ustalane) dwie pozycje
		(st¹d nazwa) w zdaniu. 
		*/
		class JOSKIPI_API CBinaryComplexC: public CComplexCond
		{
		public:
			/**
			Wymaga dok³adnie dwóch warunków sk³adowych i dwóch pozycji.

			Trzy pozycje, na których pracuj¹ obiekty tej klasy s¹ wyró¿nione nazwami: startPos, endPos i varPos.
			\throw std::string gdy !positions || (int)positions->size() != 3
			*/
			CBinaryComplexC(std::vector<pos_ptr>* positions, std::vector<CCondition*>* conditions) throw(std::string);
			virtual ~CBinaryComplexC(void);
			
			/// Pomocnicza funkcja, która mo¿e siê przydaæ dla innych operatorów.
			static void CalculatePositionLimits(Corpus::CSentence& sentence,
				Corpus::CPosition &s, Corpus::CPosition &e, const Corpus::CPosition &initS, const Corpus::CPosition &initE);

		protected:
			/**
			Dot. startPos i endPos
			WskaŸnik lub sta³a wskazuj¹ca na pozycjê w tablicy, wyró¿nia dwie (jedyne) pozycje na jakich pracuj¹ obiekty 
			klas pochodnych po danej; wartoœci tych obiektów mog¹ byæ zmieniane aby zostaæ przekazane do dalszych warunków;
			*/
			pos_ptr startPos;
			/**
			Zob. startPos
			*/
			pos_ptr endPos;
			/**
			Obiekt pozycji -- zmienna, po której bêdziemy iterowaæ wewn¹trz warunku binarnego (tzn. do kolejnych po³o¿eñ 
			tej zmiennej bêdzie stosowany sk³adowy warunek wykorzystuj¹cy t¹ zmienn¹); stan tej zmiennej (obiektu) bêdzie 
			wykorzystywany do przekazywania znalezionej pozycji na zewn¹trz (w wypadku braku znalezionej pozycji stan
			jej to CPosition::nowhere).
			*/
			pos_ptr varPos;

			/**
			Pomocnicza metoda ustalaj¹ca dla bie¿¹cego zdania granice dzia³ania operatora binarnego.

			1. if startPos.position == CPosition::beginning st=pocz¹tek zdania else st=startPos.position
			2. if endPos.position == CPosition::end end=koniec zdania  else end=startPos.position
			*/
			void CalculateLimits(Corpus::CSentence& sentence, Corpus::CPosition &s, Corpus::CPosition &e);
		};
	}
}
#endif

