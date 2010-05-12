#include "../joskipi_headers.h"
#include "RulesParser.h"


namespace JOSKIPI
{
	namespace Parsers
	{
		CRulesParser::CRulesParser(Rules::CRuleSet* rules)
			:rules(rules)
		{
			preProcessor = new std::map<std::string,std::vector<std::string> >();
			patPar = new CPatternParser(0);
		}	

		CRulesParser::~CRulesParser(void)
		{
			delete preProcessor;
			delete patPar;
		}

		void CRulesParser::AddMacro(std::vector<std::string>& macroStr)
		{
			if(preProcessor->find(Corpus::Parser::Trim(macroStr[0])) != preProcessor->end())
				throw Corpus::tagexception("Macro already defined.");

			std::vector<std::string> temp;

			for(int i = 1; i < (int)macroStr.size(); i++)
			{
				temp.push_back(macroStr[i]);
			}

			boost::algorithm::trim(macroStr[0]);
			(*preProcessor)[macroStr[0]] = temp;
		}

		void CRulesParser::InsertMacros(std::vector<std::string>& patternLines)
		{
			std::vector<std::string> temp;

			for(int i = 0; i < (int)patternLines.size(); i++)
			{
				if(patternLines[i][0] == '!')
				{
					if(preProcessor->find(patternLines[i]) == preProcessor->end())
						throw Corpus::tagexception("Macro not defined.");

					std::vector<std::string> temp2 = (*preProcessor)[patternLines[i]];
					for(int j = 0; j < (int)temp2.size(); j++)
						temp.push_back(temp2[j]);
				}
				else
					temp.push_back(patternLines[i]);
			}

			patternLines.clear();
			for(int i = 0; i < (int)temp.size(); i++)
			{
				patternLines.push_back(temp[i]);
			}
		}

		void CRulesParser::Parse(std::ifstream& inFile)
		{
			using namespace std;
			using namespace Corpus;
			preProcessor->clear();
			char* linia = new char[255];
			vector<string> ruleStrings;
			string s;

			// Zbieramy pocz�tkowe puste linie.
			do
			{
				inFile.getline(linia,255);
				s = linia;
				/*s = */boost::algorithm::trim(s);
				//s = Parser::Trim(s);
			}
			while(!inFile.eof() && s.empty());

			do
			{
				ruleStrings.clear();
				do
				{
					if(s[0] != '/')
						ruleStrings.push_back(s);
					inFile.getline(linia,255);
					s = linia;
					/*s = */boost::algorithm::trim(s);
				}
				while(!s.empty());

				if(ruleStrings[0][0] != '!')
				{
					InsertMacros(ruleStrings);
					ParseRule(ruleStrings);
				}
				else
					AddMacro(ruleStrings);

				do
				{
					inFile.getline(linia,255);
					s = linia;
					/*s = */boost::algorithm::trim(s);
				}
				while(!inFile.eof() && (s.empty() || s[0] == '/'));
			}
			while(!inFile.eof());

			delete [] linia;
		}

		void CRulesParser::ParseRule(std::vector<std::string>& ruleStrings)
		{
			using namespace std;
			using namespace Corpus;
			using JOSKIPI::Operators::CCondition;

			map<string,pos_ptr> *positions = new map<string,pos_ptr>();
			patPar->DeclarePositions(positions,ruleStrings);

			vector<string> action;
			vector<string> condition;

			int priority = 0;

			ruleStrings[0] = ruleStrings[0].substr(ruleStrings[0].find("(")+1);

			bool actionEnd = false;
			for(int i = 0; i < (int)ruleStrings.size(); i++)
			{
				if(ruleStrings[i].find("#") != string::npos)
				{
					actionEnd = true;
					vector<string> temp;
					Parser::Tokenize(ruleStrings[i],temp);

					action.push_back(temp[0]);
					priority = atoi(temp[2].c_str());
				}
				else
				{
					if(actionEnd)
						condition.push_back(ruleStrings[i]);
					else
						action.push_back(ruleStrings[i]);
				}
			}

			vector<CCondition*>* condVec = new vector<CCondition*>();
			patPar->ParseConditionLines(condition,condVec,positions);
			vector<CCondition*>* actionVec = new vector<CCondition*>();
			positions->clear();
			delete positions;
			patPar->ParseConditionLines(action,actionVec,NULL);

			//char fileName[50];

			//sprintf(fileName,".\\Rules\\Rule%d.txt",priority);
			
			Rules::CAction *new_action = new Rules::CAction((*actionVec)[0]);
			Rules::CRule *new_rule = new Rules::CRule((*condVec)[0], new_action,priority);

			rules->AddRule(new_rule);

			delete condVec;
			delete actionVec;
		}
	}//namespace JOSKIPI

}//namespace Parsers
