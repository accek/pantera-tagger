#include "../../takipi_headers.h"
#include "Ripper.h"
#include "../IO.h"

namespace TaKIPI
{
	namespace Classifiers
	{
		CRipper::CRipper(std::vector<std::string> & rulesData, Corpus::TFlexClass flexClass, Corpus::DLONG gramCat)
			: CRuleClassifier(rulesData,flexClass,gramCat)
		{	
		}

		CRipper::~CRipper(void)
		{

		}


		STATIC CRipper* CRipper::CreateClassifier(std::ifstream& inFile, Corpus::TFlexClass flexClass, 
			Corpus::DLONG gramCat)
		{
			std::vector<std::string> ripperStrings;
			std::string s;
			do
			{
				s = IO::ReadLine(inFile);
			}
			while(!inFile.eof() && (s.find("JRIP rules:") == s.npos));

			IO::ReadLine(inFile); //"======="
			IO::ReadLine(inFile); //pusta
			s = IO::ReadLine(inFile);


			while(!inFile.eof() && (s.find("Number of Rules") == s.npos))
			{
				ripperStrings.push_back(s);
				s = IO::ReadLine(inFile);
			}

			if(inFile.eof())
			{
				return NULL;
			}

			if(ripperStrings[ripperStrings.size() -1].empty())
			{
				ripperStrings.pop_back();
			}

			std::vector<std::string> tokens;
			Corpus::Parser::Tokenize(s,tokens,":");
			if(tokens.size() != 2)
				throw Corpus::tagexception("CRipper::CreateClassifier, expected line \"Number of Rules: x\"");
			int numberOfRules = atoi(tokens[1].c_str());

			if(numberOfRules != (int)ripperStrings.size())
			{
				throw Corpus::tagexception("CRipper::CreateClassifier, number of rules read does not match stated number of rules");
			}

			return new CRipper(ripperStrings,flexClass,gramCat);
		}

	}//namespace TaKIPI

}//namespace Classifiers
