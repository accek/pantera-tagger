/*  Biblioteka JOSKIPI
 *  Język Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyński
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
		Klasa dziedzicz�ca po CPosition, tak�e przedstawiaj�ca pozycje, jednak�e pozycj�, 
		kt�ra jest przesuni�ta wzgl�dem innej pozycji podawanej w konstruktorze.

		Np. je�li p wskazuje na pozycj� -1, to pozycja pOff = new COffsetPosition(-1,p),
		b�dzie wskazywa�a na pozycje -2, a gdy przesuniemy p na -2 to pOff b�dzie 
		wskazywa�o na -3.
		*/
		class JOSKIPI_API COffsetPosition :
			public Corpus::CPosition
		{
		public:
			/**
			Konstruktor przyjmuj�cy pozycje wzgl�dem kt�rej ustalamy pozycje i offset przesuni�cia

			Np. je�li p wskazuje na pozycj� -1, to pozycja pOff = new COffsetPosition(-1,p),
			b�dzie wskazywa�a na pozycje -2, a gdy przesuniemy p na -2 to pOff b�dzie 
			wskazywa�o na -3.
			\throw std::string gdy position == 0
			*/
			COffsetPosition(int offset, pos_ptr position) throw (std::string);
			/**
			Nie kasuje positionPtr
			*/
			~COffsetPosition(void);

			/// Warto�� pozycji.
			int GetPos() const;

		protected:
			/// Offset wzgl�dem innej pozycji.
			int offset;
			/// Pozycja wzgl�dem kt�rej ustalamy pozycj� obiektu.
			pos_ptr positionPtr;
		};
	}
}
#endif

