/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_DISOPER_H
#define LIBJOSKIPI_DISOPER_H

#include "../joskipi_headers.h"
#include "Operator.h"


namespace JOSKIPI
{
	namespace Operators
	{
		/**
		\todo Opis CDisPosOper 
		*/
		class JOSKIPI_API CDisPosOper :	public COperator
		{
		public:
			/**
			\throw std::string gdy position == 0
			*/
			CDisPosOper(pos_ptr position) throw(std::string);
			CDisPosOper(const CDisPosOper& op);
			~CDisPosOper(void);

			const CDisPosOper& operator =(const CDisPosOper& op);

			bool GetValue(std::vector<Corpus::CValue>& val, Corpus::CSentence& sentence);
			void GetValueNames(std::stringstream& out);

			std::string ToString();

		protected:
			// Tablica wyprowadzonych wartoœci (binarnych i stringowych).
			std::vector<Corpus::DLONG>* values;
		};

		//-----------------------------------------------------------------------------

		/**
		\todo Opis CDisCatOper 
		*/
		class JOSKIPI_API CDisCatOper :
			public COperator
		{
		public:
			/**
			\throw std::string gdy position == 0
			*/
			CDisCatOper(pos_ptr position)throw(std::string);
			CDisCatOper(const CDisCatOper& op);
			~CDisCatOper(void);

			const CDisCatOper& operator =(const CDisCatOper& op);

			bool GetValue(std::vector<Corpus::CValue>& val, Corpus::CSentence& sentence);
			void GetValueNames(std::stringstream& out);

			std::string ToString();

		protected:
			// Tablica wyprowadzonych wartoœci (binarnych i stringowych).
			std::vector<Corpus::DLONG>* values;
		};
	}
}
#endif // LIBJOSKIPI_DISOPER_H

