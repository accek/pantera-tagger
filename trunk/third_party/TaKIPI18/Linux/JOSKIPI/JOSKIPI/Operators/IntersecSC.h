/*  Biblioteka JOSKIPI
 *  JÄ™zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef LIBJOSKIPI_INTERSEC_H
#define LIBJOSKIPI_INTERSEC_H

#include "../joskipi_headers.h"
#include "SimpleCondition.h"

namespace JOSKIPI
{
	namespace Operators
	{
		/**
		Klasa binarnego testu czêœci wspólnej wartoœci/zbioru pierwszego argumentu (tj. cndElements[0]) z wartoœci¹ drugiego 
		(dla pojedynczych wartoœci binarnych oznacza to wystêpowanie wspólnych bitów).
		*/
		class JOSKIPI_API CIntersecSC :
			public CSimpleCondition
		{
		public:
			/**
			\throw std::string gdy cndElems.size() != 2
			*/
			CIntersecSC(std::vector<CCondElem*>& cndElems) throw (std::string);
			virtual ~CIntersecSC(void);

			/**

			1. Przekazane zdanie przekazuje dalej do metod GetValue obiektów z wektora cndElements i wartoœci s¹ 
			zapisywane do obiektów z tablicy valBufTab. <br>
			2. if tylko jedna wartoœæ w valBufTab[0] i valBufTab[0] (porównujemy wartoœci wyra¿one na jednym int, ale ci¹gle 
			mog¹ to byæ sumy bitowej)<br>
			(a) if istniej¹ wspólne bity pierwszej i drugiej wartoœci lub dwie wartoœci stringowe s¹ identyczne 
			return true else return false <br>
			2. else <br>
			(a)if istniej¹ wartoœci z valBufTab[0], które znajduj¹ siê w valBufTab[1] return true else return false.<br>
			*/
			virtual bool Evaluate(Corpus::CSentence& sentence);

			virtual std::string ToString();
		};
	}
}
#endif // LIBJOSKIPI_INTERSEC_H

