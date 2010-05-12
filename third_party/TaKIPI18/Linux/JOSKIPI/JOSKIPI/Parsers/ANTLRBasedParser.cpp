#include "../joskipi_headers.h"
#include "ANTLRBasedParser.h"

namespace JOSKIPI
{
	namespace Parsers
	{
		CAntlrParser::CAntlrParser(bool ruleDumpMode)
			:dumpRules(ruleDumpMode)
		{
		}
		void CAntlrParser::ParseOperator(const std::string &s, std::vector<Operators::COperator*>& operators)
		{
			std::stringstream str;
			str << s;
			antlrgen::ANTLRLexer lex(str);
			antlrgen::ANTLRParser parser(lex);

			parser.Initalize();

			Operators::COperator* result = parser.operator_joskipi();
			operators.push_back(result);
		}
		Operators::CCondition* CAntlrParser::ParseCondition(const std::string &s)
		{
			std::stringstream str;
			str << s;
			antlrgen::ANTLRLexer lex(str);
			antlrgen::ANTLRParser parser(lex);

			parser.Initalize();

			return parser.testop();
		}
		
		void CAntlrParser::ParseRules(std::istream &str, Rules::CRuleSet *rs)
		{
			antlrgen::ANTLRLexer lex(str);
			antlrgen::ANTLRParser parser(lex);

			parser.Initalize();
			parser.SwitchRuleDumpMode(dumpRules);

			parser.reguly(rs);
		}
		
		void CAntlrParser::ParsePatterns(std::istream &str, std::map<Corpus::CValue,Patterns::CPattern*> *patterns)
		{
			antlrgen::ANTLRLexer lex(str);
			antlrgen::ANTLRParser parser(lex);
			parser.Initalize();
			parser.wzorce(patterns);
		}
		
		void CAntlrParser::ParsePatterns(const std::string &s, std::map<Corpus::CValue,Patterns::CPattern*> *patterns)
		{
			std::stringstream str;
			str << s;
			ParsePatterns(str, patterns);
		}
		
		void CAntlrParser::ParseRules(const std::string &s, Rules::CRuleSet *rs)
		{
			std::stringstream str;
			str << s;
			ParseRules(str, rs);
		}
	}//namespace Parsers
}//namespace JOSKIPI


