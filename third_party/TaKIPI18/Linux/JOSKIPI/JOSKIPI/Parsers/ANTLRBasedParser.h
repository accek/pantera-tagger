/*  Biblioteka JOSKIPI
 *  J�zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc�awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardy�ski
 *
 *  Kod na licencji GPL.
 */

#ifndef JOSKIPI_ANTLRPARSER_H
#define JOSKIPI_ANTLRPARSER_H

#include "antlr/ANTLRLexer.hpp"
#include "antlr/ANTLRParser.hpp"

#include "../joskipi_headers.h"
#include "../all_operators.h"
#include "../all_rules.h"

namespace JOSKIPI
{
	namespace Parsers
	{
		class JOSKIPI_API CAntlrParser
		{
		public:
			/// Creates a parser. When ruleDumpMode, rules are created with default logging output (quite verbose).
			CAntlrParser(bool ruleDumpMode = false);
			void ParseOperator(const std::string &s, std::vector<Operators::COperator*> &operators);
			Operators::CCondition* ParseCondition(const std::string &s);
			void ParseRules(const std::string &s, Rules::CRuleSet *rs);
			void ParsePatterns(const std::string &s, std::map<Corpus::CValue,Patterns::CPattern*> *patterns);
			/// Parses more effectively: directly from stream. Stores into patterns object.
			void ParsePatterns(std::istream &str, std::map<Corpus::CValue,Patterns::CPattern*> *patterns);
			/// Parses more effectively: directly from stream. Stores into rs object.
			void ParseRules(std::istream &str, Rules::CRuleSet *rs);
		private:
			bool dumpRules;
		};
	}//namespace Parsers
}//namespace JOSKIPI

#endif // JOSKIPI_ANTLRPARSER_H

