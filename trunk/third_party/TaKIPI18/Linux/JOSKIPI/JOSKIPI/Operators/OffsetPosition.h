/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_OFFSETPOSITION_H
#define LIBJOSKIPI_OFFSETPOSITION_H

#include "../joskipi_headers.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Klasa dziedzicz¹ca po CPosition, tak¿e przedstawiaj¹ca pozycje, jednak¿e pozycjê, 
		która jest przesuniêta wzglêdem innej pozycji podawanej w konstruktorze.

		Np. jeœli p wskazuje na pozycjê -1, to pozycja pOff = new COffsetPosition(-1,p),
		bêdzie wskazywa³a na pozycje -2, a gdy przesuniemy p na -2 to pOff bêdzie 
		wskazywa³o na -3.
		*/
		class JOSKIPI_API COffsetPosition :
			public Corpus::CPosition
		{
		public:
			/**
			Konstruktor przyjmuj¹cy pozycje wzglêdem której ustalamy pozycje i offset przesuniêcia

			Np. jeœli p wskazuje na pozycjê -1, to pozycja pOff = new COffsetPosition(-1,p),
			bêdzie wskazywa³a na pozycje -2, a gdy przesuniemy p na -2 to pOff bêdzie 
			wskazywa³o na -3.
			\throw std::string gdy position == 0
			*/
			COffsetPosition(int offset, pos_ptr position) throw (std::string);
			/**
			Nie kasuje positionPtr
			*/
			~COffsetPosition(void);

			/// Wartoœæ pozycji.
			int GetPos() const;

		protected:
			/// Offset wzglêdem innej pozycji.
			int offset;
			/// Pozycja wzglêdem której ustalamy pozycjê obiektu.
			pos_ptr positionPtr;
		};
	}
}
#endif

