#include "../joskipi_headers.h"
#include "PatternParser.h"


namespace JOSKIPI
{
	namespace Parsers
	{

		CPatternParser::CPatternParser(std::map<Corpus::CValue,Patterns::CPattern*>* patterns)
			: patterns(patterns), JOSKIPI::Parsers::CSimpleParser()
		{
			preProcessor = new std::map<std::string,std::vector<std::string> >();
		}

		CPatternParser::~CPatternParser(void)
		{
			if(preProcessor)
			{
				delete preProcessor;
				preProcessor = NULL;
			}
		}


		void CPatternParser::AddMacro(std::vector<std::string>& macroStr)
		{
			// jeœli makro o danej nazwie ju¿ istnieje, rzuca wyj¹tek
			if(preProcessor->find(Corpus::Parser::Trim(macroStr[0])) != preProcessor->end())
				throw Corpus::tagexception("Macro already defined.");

			std::vector<std::string> temp;	// zostanie ustawiony na wszystko oprócz
											// nazwy identyfikuj¹cej makro

			for(int i = 1; i < (int)macroStr.size(); i++)
			{
				temp.push_back(macroStr[i]);
			}

			// macroStr[0] - nazwa identyfikuj¹ca makro (³¹cznie z wykrzyknikiem)
			(*preProcessor)[Corpus::Parser::Trim(macroStr[0])] = temp;
		}

		void CPatternParser::InsertMacros(std::vector<std::string>& patternLines)
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

		void CPatternParser::Parse(std::istream &inFile)
		{
			preProcessor->clear();
			char* linia = new char[255];
			std::vector<std::string> patternStrings;
			std::string s;

			// Zbieramy pocz¹tkowe puste linie.
			do
			{
				inFile.getline(linia,255);
				s = linia;
				s = Corpus::Parser::Trim(s);
			}
			while(!inFile.eof() && Corpus::Parser::Trim(s).empty());

			

			do
			{
				patternStrings.clear();
				do
				{
					if(s[0] != '/')
						patternStrings.push_back(s);
					inFile.getline(linia,255);
					s = linia;
					s = Corpus::Parser::Trim(s);
				}
				while(!(Corpus::Parser::Trim(s).empty()));	// zbiera linie do pierwszej
				// pustej (pomija komentarze) i umieszcza je w patternStrings

				if(patternStrings[0][0] != '!')	// jeœli blok napisów oznacza wzorzec
				{
					InsertMacros(patternStrings);
					ParsePattern(patternStrings);
				}
				else	// jeœli blok napisów oznacza makro
					AddMacro(patternStrings);

				do	// pomija puste linie oraz komentarze
				{
					inFile.getline(linia,255);
					s = linia;
					s = Corpus::Parser::Trim(s);
				}
				while(!inFile.eof() && ( (Corpus::Parser::Trim(s).empty()) || Corpus::Parser::Trim(s)[0] == '/'));
			}
			while(!inFile.eof());

			delete [] linia;
		}

		/* Parsuje blok napisów, oznaczaj¹cy pojedynczy wzorzec */
		void CPatternParser::ParsePattern(std::vector<std::string>& patternStrings)
		{
			using namespace Corpus;
			using namespace std;
			int pointer = 0;
			CValue ambClass(0,0);

			vector<string> ambClassStrings;
			Parser::Tokenize(patternStrings[0],ambClassStrings," :");

			for(int i = 0; i < (int)ambClassStrings.size()-1; i++)	// ustawianie
			{														// CValue ambClass
				ambClass.first |= CTag::GetFlexClass(ambClassStrings[i]);
				ambClass.second |= CTag::GetGrammCatValue(ambClassStrings[i]);
			}

			// jeœli istnieje ju¿ zdefiniowany wzorzec dla danej klasy wieloznacznoœci,
			// wystêpuje wyj¹tek (taka sytuacja nie powinna mieæ miejsca)
			if(patterns->find(ambClass) != patterns->end())
			{
				char buffer[256];
				sprintf(buffer,"Pattern %l/%l already defined.\0",ambClass.first,ambClass.second);
				throw tagexception(buffer);
			}

			pointer++;

			vector<JOSKIPI::Operators::COperator*>* operators = new vector<JOSKIPI::Operators::COperator*>();

			vector<string> opLines;
			int andOrNot = 0;

			do
			{
				opLines.clear();

				if(patternStrings[pointer].find("and(") == patternStrings[pointer].npos
					&& patternStrings[pointer].find("or(") == patternStrings[pointer].npos
					&& patternStrings[pointer].find("not(") == patternStrings[pointer].npos)
				{
					opLines.push_back(Parser::Trim(patternStrings[pointer]));
					pointer++;
				}
				else
				{
					do
					{
						if(Parser::Trim(patternStrings[pointer])[0] == ')')
							andOrNot--;

						if(patternStrings[pointer].find("and(") != patternStrings[pointer].npos
							|| patternStrings[pointer].find("or(") != patternStrings[pointer].npos
							|| patternStrings[pointer].find("not(") != patternStrings[pointer].npos)
							andOrNot++;
						opLines.push_back(Parser::Trim(patternStrings[pointer]));
						pointer++;
					}
					while(andOrNot != 0);
				}
				ParseOperatorLines(opLines,operators);
			}
			while(pointer != (int)patternStrings.size());

			(*patterns)[ambClass] = new Patterns::CPattern(operators,ambClass.first,ambClass.second);
		}

	}//namespace JOSKIPI

}//namespace Parsers
