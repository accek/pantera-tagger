#include "../../corpuslib_headers.h"
#include "Core.h"
#include "../Config.h"

namespace Corpus
{
	namespace Tokenisation
	{
		std::string SPretokenRule::lastErrorMessage;
		
		bool SPretokenRule::Classify(SPretoken &pretoken)
		{
			try
			{
				if(boost::regex_match(pretoken.cp_orth, condition))
				{
					pretoken.flexClass = tag.FlexClass;
					pretoken.attrValues = tag.Categories;
					return true;
				}
			}
			catch(const std::exception &)
			{
				/* In case of boost::regex error: "memory exhausted".
				It may happed when a regular expression generates
				an automaton which needs too much resources for
				checking current match. It's better to catch it than
				just to fail and terminate tokenisation. */
				/*
				std::cerr << "Warning: " << e.what()
						<< " (in tokeniser regexp for "
						<< tag.ToString() << ")\n";
				*/
			}

			return false;
		}

		SPretokenRule *SPretokenRule::ParseContent(std::istream &content)
		{
			SPretokenRule *toReturn = NULL;
			std::string part;
			content >> part;
			CTag tag;
			if(!part.empty())
			{
				try
				{
					tag.ParseContent(part);
					content >> part; // :-
					if(part.compare(":-") == 0)
					{
						content >> part; // regex
						toReturn = new SPretokenRule(tag, part);
					}
				}
				catch(tagexception &e)
				{
					std::stringstream msg;
					msg << e.what() << " at SPretokenRule::ParseContent";
					lastErrorMessage = msg.str();
				}
			}
			return toReturn;
		}

		CCore::CCore()
		{
			LoadClassificationRules();
			LoadSeparators();
		}
		
		CCore::~CCore()
		{
			ClearClassificationRules();
		}
		
		bool CCore::Classify(SPretoken &pretoken)
		{
			for(size_t pos = 0; pos < rules.size(); pos++)
			{
				if(rules[pos]->Classify(pretoken))
				{
					return true;
				}
			}
			return false;
		}

		void CCore::LoadClassificationRules()
		{
			std::ifstream ruleFile(CConfig::GetTokeniserRulePath().c_str());
			if(ruleFile)
			{
				SPretokenRule *rule;
				while((rule = SPretokenRule::ParseContent(ruleFile)) != NULL)
				{
					rules.push_back(rule);
				}
			}
		}
		
		void CCore::ClearClassificationRules()
		{
			for(size_t pos = 0; pos < rules.size(); pos++)
			{
				delete rules[pos];
			}
			rules.clear();
		}

		void CCore::LoadSeparators()
		{
			for(size_t indx = 0; indx < 256; indx++)
			{
				begSepMask[indx] = false;
				endSepMask[indx] = false;
				alwSepMask[indx] = false;
			}

			std::ifstream file(CConfig::GetTokeniserSepPath().c_str());
			std::string part;
			int mode = 0;
			while(file)
			{
				file >> part;
				if(!file) break;

				if(!part.empty())
				{
					if(part.compare("BEGINSEPARATORS") == 0)
					{
						mode = 1;
					}
					else if(part.compare("ENDSEPARATORS") == 0)
					{
						mode = 2;
					}
					else if(part.compare("ALWAYSSEPARATORS") == 0)
					{
						mode = 3;
					}
					else
					{
						int code = 0;
						if(part.size() == 1)
						{
							code = ((unsigned char)part[0]);
						}
						else if(part[0] == '#')
						{
							std::stringstream codestr;
							codestr.setf(std::ios::hex, std::ios::basefield);
							codestr << part.substr(1);
							codestr >> code;
						}
						if(code >= 32 && code < 256)
						{
							if(mode == 1)
							{
								begSepMask[code] = true;
							}
							else if(mode == 2)
							{
								endSepMask[code] = true;
							}
							else if(mode == 3)
							{
								alwSepMask[code] = true;
							}
						}
					}
				}
			}
		}
		
		bool CCore::IsBlankUniversal(char ch)
		{
			return (
					(((unsigned char)ch) <= 32) // control char
					|| (((unsigned char)ch) == 127) // "DELETE"
				   );
		}
		
	} // namespace Tokenisation
} // namespace Corpus
