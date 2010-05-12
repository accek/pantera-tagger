#include "../../takipi_headers.h"
#include "Part.h"
#include "../IO.h"

namespace TaKIPI
{
	namespace Classifiers
	{

		CPart::CPart(std::vector<std::string> & rulesData, Corpus::TFlexClass flexClass, Corpus::DLONG gramCat)
			: CRuleClassifier(rulesData,flexClass,gramCat)
		{
		}

		CPart::~CPart(void)
		{
		}

		STATIC CPart* CPart::CreateClassifier(std::ifstream& inFile, Corpus::TFlexClass flexClass, Corpus::DLONG gramCat)
		{
			std::vector<std::string> partStrings;
			std::string s;
			do
			{
				s = IO::ReadLine(inFile);
			}
			while(!inFile.eof() && (s.find("PART decision list") == s.npos));

			IO::ReadLine(inFile); //"------"
			IO::ReadLine(inFile); //pusta
			s = IO::ReadLine(inFile);


			while(!inFile.eof() && (s.find("Number of Rules") == s.npos))
			{
				partStrings.push_back(s);
				s = IO::ReadLine(inFile);
			}

			if(inFile.eof())
			{
				return NULL;
			}

			if(partStrings[partStrings.size() -1].empty())
			{
				partStrings.pop_back();
			}

			std::vector<std::string> tokens;
			Corpus::Parser::Tokenize(s,tokens,":");
			if(tokens.size() != 2)
				throw Corpus::tagexception("CPart::CreateClassifier, expected line \"Number of Rules: x\"");
			int numberOfRules = atoi(tokens[1].c_str());

			if((size_t)numberOfRules != partStrings.size())
			{
				throw Corpus::tagexception("CPart::CreateClassifier, number of rules read does not match stated number of rules");
			}

			return new CPart(partStrings,flexClass,gramCat);
		}

	}//namespace TaKIPI

}// namespace Classifiers
