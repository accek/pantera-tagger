/*  Biblioteka JOSKIPI
 *  J�zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc�awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardy�ski
 *
 *  Kod na licencji GPL.
 */

#ifndef JOSKIPI_RULEPARSER_H
#define JOSKIPI_RULEPARSER_H

#include "../joskipi_headers.h"
#include "PatternParser.h"
#include "../Rules/RuleSet.h"

namespace JOSKIPI
{
	namespace Parsers
	{
		/**  DEPRECATED. Zalecany CAntlrParser. */
		class JOSKIPI_API CRulesParser
		{
		public:
			CRulesParser(Rules::CRuleSet* rules);
			~CRulesParser(void);

			// parsuje plik z regu�ami, kt�re zapisuje do zbioru regu� podawanego
			// w konstruktorze
			void Parse(std::ifstream& file);

		protected:
			void AddMacro(std::vector<std::string>& macroStr);
			void InsertMacros(std::vector<std::string>& patternLines);
			void ParseRule(std::vector<std::string>& ruleStrings);

			Rules::CRuleSet* rules;
			std::map<std::string,std::vector<std::string> >* preProcessor;

			CPatternParser* patPar;
		};

	}//namespace JOSKIPI

}//namespace Parsers
#endif //JOSKIPI_RULEPARSER_H

