#include "../../corpuslib_headers.h"
#include "Core.h"
#include "../Config.h"
#include "../Parser/Parser.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		CCore::CCore()
		{
			LoadAbbrevList();
		}

		bool CCore::IsTokAbbrev(const std::string &cp_orth)
		{
			std::string cp_lower_orth(cp_orth);
			Parser::ToLowerCP1250(cp_lower_orth);
			return tok_abbrs.find(cp_lower_orth) != tok_abbrs.end();
		}

		bool CCore::IsSymAbbrev(const std::string &cp_orth)
		{
			std::string cp_lower_orth(cp_orth);
			Parser::ToLowerCP1250(cp_lower_orth);
			return sym_abbrs.find(cp_lower_orth) != sym_abbrs.end();
		}

		void CCore::LoadAbbrevList()
		{
			LoadList(CConfig::GetAbbrListPath(), tok_abbrs);
			LoadList(CConfig::GetAbbrSymListPath(), sym_abbrs);
		}

		bool CCore::LoadList(const std::string &path, std::set<std::string> &abbrSet)
		{
			std::ifstream listFile(path.c_str());
			if(!listFile)
				return false;
			std::string abbrev;
			while(std::getline(listFile, abbrev))
			{
				abbrSet.insert(abbrev);
			}
			return true;
		}

	} // namespace ExtSentencer
} // namespace Corpus
