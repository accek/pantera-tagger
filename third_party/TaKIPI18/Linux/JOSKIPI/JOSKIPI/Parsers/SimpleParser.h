/*  Biblioteka JOSKIPI
 *  Jêzyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc³awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardyñski
 *
 *  Kod na licencji GPL.
 */

// UNIX
#ifndef _INCLUDED_PATTERNPARSER_H_
#define _INCLUDED_PATTERNPARSER_H_

#include "../joskipi_headers.h"
#include "../all_operators.h"


namespace JOSKIPI
{
	namespace Parsers
	{
		/** DEPRECATED. Zalecany CAntlrParser.
		Prosty parser do operatorów JOSKIPI stworzony z CPatternParser Grzegorza Godlewskiego
		\todo Opis uzycia
		
		*/
		class JOSKIPI_API CSimpleParser
		{
		public:
			CSimpleParser();
			~CSimpleParser(void);

			/**
			\todo dodano usuwanie stworzonych pozycji... sprawdzic czy sie nie wysypie
			*/
			void ParseOperatorLines(std::vector<std::string>& opLines, std::vector<Operators::COperator*>* operators,
				std::map<std::string,pos_ptr>* positions = 0);
			void ParseConditionLines(std::vector<std::string>& condLines, std::vector<Operators::CCondition*>* conditions, 
				std::map<std::string,pos_ptr>* positions);
			void DeclarePositions(std::map<std::string,pos_ptr>* positions, std::vector<std::string>& opLines);

			void ParseSingleOperator(std::string& oper, std::vector<Operators::COperator*>* operators, 
				std::map<std::string,pos_ptr>* positions);

			Operators::COperator* ParseSimpleOperator(std::string name, std::string posString, 
				std::map<std::string,pos_ptr>* positions);

			Operators::CEqualSC*		ParseEqual(std::string& oper,std:: map<std::string,pos_ptr>* positions);
			Operators::CInSC*			ParseIn(std::string& oper, std::map<std::string,pos_ptr>* positions);
			Operators::CIntersecSC*	ParseIntersec(std::string& oper, std::map<std::string,pos_ptr>* positions);
			Operators::CIsBigSC*		ParseIsBig(std::string& oper, std::map<std::string,pos_ptr>* positions);
			Operators::CHasNumSC*		ParseHasNum(std::string& oper, std::map<std::string,pos_ptr>* positions);

			Operators::COperator*		ParseFilter(std::string& oper, 
				std::map<std::string,pos_ptr>* positions);

			Operators::CComplexCond*	ParseAndOrNot(std::vector<std::string>& opLines, 
				std::map<std::string,pos_ptr>* positions);

			Operators::CBinaryComplexC* ParseOnlyLook(std::vector<std::string>& opLines, 
				std::map<std::string,pos_ptr>* Positions);
			Operators::CBinaryComplexC* ParseAgreement(std::vector<std::string>& opLines, 
				std::map<std::string,pos_ptr>* Positions);

			Corpus::CValue ParseConst(std::string& constStr);
			Operators::CConstValue* ParseStringsConst(std::string& constString);
		};

	}
}
#endif

