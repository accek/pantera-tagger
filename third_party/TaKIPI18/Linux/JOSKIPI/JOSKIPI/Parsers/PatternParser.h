/*  Biblioteka JOSKIPI
 *  J�zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc�awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardy�ski
 *
 *  Kod na licencji GPL.
 */

#ifndef JOSKIPI_PATTERNPARSER_H
#define JOSKIPI_PATTERNPARSER_H

#include "../joskipi_headers.h"
#include "../Patterns/Pattern.h"
#include "SimpleParser.h"

namespace JOSKIPI
{
	namespace Parsers
	{
		/** DEPRECATED. Zalecany CAntlrParser.
		Klasa transformuj�ca (parsuj�ca) stworzony do cel�w projektu j�zyk operator�w JOSKIPI, do obiekt�w wzorca.
		*/
		class JOSKIPI_API CPatternParser : public JOSKIPI::Parsers::CSimpleParser
		{
		public:
			/*
			Konstruktor, gdzie podajemy map�, do kt�rej b�dziemy wrzuca� sparsowane wzorce, oraz managera pami�ci do operator�w.
			*/
			CPatternParser(std::map<Corpus::CValue,Patterns::CPattern*>* patterns);
			~CPatternParser(void);

			/*
			G��wna metoda parsuj�ca, przyjmuj�ca strumie� wej�ciowy (najcz�ciej plik), z kt�rego parsowane s� 
			wzorce (operatory i warunki) JOSKIPI. Wzorzec wraz z operatorami jest dodawany do mapy wzorc�w podawanych w konstruktorze.
			*/
			void Parse(std::istream& file);

		protected:
			/*
			PREPROCESOR: Dodaje makro do wewn�trznej mapy makr.
			*/
			void AddMacro(std::vector<std::string>& macroStr);
			/* 
			PREPROCESOR: Zamienia makra na kod.
			*/
			void InsertMacros(std::vector<std::string>& patternLines);

			void ParsePattern(std::vector<std::string>& patternStrings);
			/*void ParseOperatorLines(vector<string>& opLines, vector<COperator*>* operators,
				map<string,CPosition*>* positions = 0);
			void ParseConditionLines(vector<string>& condLines, vector<CCondition*>* conditions, map<string,CPosition*>* positions);
			void DeclarePositions(map<string,CPosition*>* positions, vector<string>& opLines);

			void ParseSingleOperator(string& oper, vector<COperator*>* operators, map<string,CPosition*>* positions);
			COperator* ParseSimpleOperator(string name, string posString, 
				map<string,CPosition*>* positions);

			CEqualSC*		ParseEqual(string& oper, map<string,CPosition*>* positions);
			CInSC*			ParseIn(string& oper, map<string,CPosition*>* positions);
			CIntersecSC*	ParseIntersec(string& oper, map<string,CPosition*>* positions);
			CIsBigSC*		ParseIsBig(string& oper, map<string,CPosition*>* positions);
			CHasNumSC*		ParseHasNum(string& oper, map<string,CPosition*>* positions);

			COperator*		ParseFilter(string& oper, map<string,CPosition*>* positions);

			CComplexCond*	ParseAndOrNot(vector<string>& opLines, map<string,CPosition*>* positions);

			CBinaryComplexC* ParseOnlyLook(vector<string>& opLines, map<string,CPosition*>* Positions);
			CBinaryComplexC* ParseAgreement(vector<string>& opLines, map<string,CPosition*>* Positions);

			CValue ParseConst(string& constStr);
			CConstValue* ParseStringsConst(string& constString);*/

			/*
			Zewn�trznie podawana mapa do przechowywania sparsowanych wzorc�w.
			*/
			std::map<Corpus::CValue,Patterns::CPattern*>* patterns;
			/*
			Manager do pami�ci (tworzonych operator�w).
			*/
			//COperMemManager* operMgr;
			/*
			Mapa makr.
			*/
			std::map<std::string,std::vector<std::string> >* preProcessor;
		};
	}//namespace JOSKIPI

}//namespace Parsers
#endif //JOSKIPI_PATTERNPARSER_H

