#include "joskipi_headers.h"

#ifdef WIN32

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	return TRUE;
}

#endif // WIN32

#ifdef TEST_WITH_MAIN

#include "all_joskipi.h"

int main(int argc, char *argv[])
{
	std::cout << "Test" << std::endl;
	try
	{
		JOSKIPI::Parsers::CAntlrParser parser;
		JOSKIPI::Parsers::CSimpleParser simpleparser;

		std::vector<std::string> lines;
		std::string line;
		std::stringstream str;
		std::ifstream file( argv[1] );
		std::ifstream oldfile( argv[2] );
		if(!file)
			throw std::runtime_error("Cannot open file for reading: " + std::string(argv[1]) );

		while(file)
		{
			std::getline(file, line);
			str << line << std::endl;
			lines.push_back(line);
		}

#pragma region operators
		/*
		std::cout << "\nParsing operator...\n\n";

		std::vector<JOSKIPI::Operators::COperator*> out;
		std::cout << "ANTLR based parser... " << argv[1] << std::endl;
		parser.ParseOperator(str.str(), out);
		std::ofstream byNewParser( "op1.txt" );
		byNewParser << out[0]->ToString() << std::endl;
		std::cout << "Results saved to op1.txt\n\n";
		byNewParser.close();
		out.clear();
		std::cout << "Old parser... " << argv[2] << std::endl;
		simpleparser.ParseOperatorLines(lines, &out);
		std::ofstream byOldParser( "op2.txt" );
		byOldParser << out[0]->ToString() << std::endl;
		std::cout << "Results saved to op2.txt\n";
		byOldParser.close();
		//*/
#pragma endregion testowanie operatorów

#pragma region rules
		/*
		std::cout << "\nParsing rules...\n\n";

		std::cout << "ANTLR based parser... " << argv[1] << std::endl;
		JOSKIPI::Rules::CRuleSet *rs = parser.ParseRules(str.str());
		std::ofstream byNewParser( "rul1.txt" );
		byNewParser << rs->ToString() << std::endl;
		std::cout << "Results saved to rul1.txt\n\n";
		byNewParser.close();
		delete rs;
		
		std::cout << "Old parser... " << argv[2] << std::endl;
		std::vector<JOSKIPI::Rules::CRule*> *rules_vector = new std::vector<JOSKIPI::Rules::CRule*>();
		rs = new JOSKIPI::Rules::CRuleSet(rules_vector);
		JOSKIPI::Parsers::CRulesParser rulPars(rs, framework.GetMemManager());
		rulPars.Parse(oldfile);
		std::ofstream byOldParser( "rul2.txt" );
		byOldParser << rs->ToString() << std::endl;
		std::cout << "Results saved to rul2.txt\n";
		byOldParser.close();
		//*/
#pragma endregion testowanie regu³

#pragma region patterns
		//*
		std::cout << "\nParsing patterns...\n\n";

		
		std::cout << "ANTLR based parser... " << argv[1] << std::endl;
		std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*> *patterns = parser.ParsePatterns(str.str());
		std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>::const_iterator it = patterns->begin();
		std::ofstream byNewParser( "pat1.txt" );
		while(it != patterns->end())
		{
			//std::cout << it->first.first;
			//std::cout << " : " << it->second->ToString() << std::endl;
			byNewParser << it->first.first;
			byNewParser << " : " << it->second->ToString() << std::endl;
			it++;
		}
		std::cout << "Results saved to pat1.txt\n\n";
		byNewParser.close();

		std::cout << "Old parser... " << argv[2] << std::endl;
		std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*> *oldpatterns = new std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>();
		JOSKIPI::Parsers::CPatternParser pattPars(oldpatterns, framework.GetMemManager());
		pattPars.Parse(oldfile);
		std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*>::const_iterator oldit = oldpatterns->begin();
		std::ofstream byOldParser( "pat2.txt" );
		while(oldit != oldpatterns->end())
		{
			//std::cout << oldit->first.first;
			//std::cout << " : " << oldit->second->ToString() << std::endl;
			byOldParser << oldit->first.first;
			byOldParser << " : " << oldit->second->ToString() << std::endl;
			oldit++;
		}
		std::cout << "Results saved to pat2.txt\n";
		byOldParser.close();
		//*/
#pragma endregion testowanie wzroców
	}
	catch(std::exception &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	catch(antlr::RecognitionException e)
	{
		std::cerr << "Antlr recognition exception: " << e.toString() << std::endl;
	}
	catch(std::string err)
	{
		std::cerr << "Exception: " << err << std::endl;
	}
}

#endif //TEST_WITH_MAIN

/*#include "joskipi_headers.h"

#ifdef WIN32

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	return TRUE;
}

#endif // WIN32

#ifdef TEST_WITH_MAIN

#include "JOSKIPI/all_joskipi.h"

int main()
{
	try
	{
		JOSKIPI::OperFramework::COperFramework  framework;
		JOSKIPI::Parsers::OperatorsParser parser(framework.GetMemManager());

		JOSKIPI::Parsers::CSimpleParser simpleparser(framework.GetMemManager());

		std::vector<std::string> lines;
		lines.push_back("and(");
		lines.push_back("llook(-2,begin,$P,equal(flex[$P],{prep})),");
		lines.push_back("inter(cas[0],cas[$P]),");
		lines.push_back("only($+1P,-1,$PL, in(flex[$PL],{adj,pact,ppas})),");
		lines.push_back("agr($+1P,0,{cas,nmb,gnd},3)");
		lines.push_back(")");
		std::stringstream str;
		for(size_t i = 0; i < lines.size(); i++)
			str << lines[i] << std::endl;

		std::cout << "Parsowanie operatora:\n " << str.str() << std::endl;

		std::vector<JOSKIPI::Operators::COperator*> out;

		simpleparser.ParseOperatorLines(lines, &out);


		std::cout << "Stary parser:\n " << out[0]->ToString() << std::endl;
		out.clear();
		parser.Parse(str.str(), out);
		std::cout << "Nowy parser:\n " << out[0]->ToString() << std::endl;
	}
	catch(std::exception &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	catch(antlr::RecognitionException e)
	{
		std::cerr << "Antlr recognition exception: " << e.toString() << std::endl;
	}
	catch(std::string err)
	{
		std::cerr << "Exception: " << err << std::endl;
	}




}

#endif //TEST_WITH_MAIN

*/
