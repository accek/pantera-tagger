#include "../corpuslib_headers.h"
#include "Config.h"
#include "Readers/Reader.h"

#ifndef GUESSER_DATA_PATH
#define GUESSER_DATA_PATH "Data/"
#endif

namespace Corpus
{
	Parser::Encoding CConfig::inputEncoding = Parser::utf8;
	CConfig::FileType CConfig::inputType = CConfig::preMorph;

	TFlexClass CConfig::flexClassesToGuess =
		cf_subst | cf_depr | cf_adj | cf_adja | cf_adjp | cf_adv | cf_fin | cf_praet
		| cf_impt | cf_imps | cf_inf | cf_pcon | cf_pant | cf_ger | cf_pact | cf_ppas;

	std::string CConfig::directories = GUESSER_DATA_PATH;
	
	std::string CConfig::endingTreePath = "ETree.bin";
	std::string CConfig::endingTagSetPath = "ETS.bin";
	std::string CConfig::dictionaryPath = "Dict.bin";
	std::string CConfig::baseEndingDictPath = "BEDict.bin";
	std::string CConfig::testLogPath = "testlog.txt";
	std::string CConfig::abbrListPath = "abbr.txt";
	std::string CConfig::abbrSymListPath = "abbr_sym.txt";
	std::string CConfig::tokeniserRulePath = "tokr_rules.txt";
	std::string CConfig::tokeniserSepPath = "tokr_separators.txt";
	std::string CConfig::prefixPath = "prefixes.txt";

	int CConfig::guesserUsage = GuessingSystem::FormsIdentifier::guessAll;

	std::string CConfig::GetFullPath(std::string& filename)
	{
        std::vector<std::string> dirs;
        boost::split(dirs, directories, boost::is_any_of(":"));
        for (std::vector<std::string>::iterator i = dirs.begin();
                i != dirs.end(); ++i) {
            std::string candidate = *i + "/" + filename;
            std::ifstream f(candidate.c_str());
            if (f)
                return candidate;
        }

        // If file not found, return the first directory.
		return dirs[0] + filename;
	}
	
	void CConfig::SetPathsToDirectory(const std::string &dir)
	{
		directories = dir;
		size_t len = dir.size();
			
		if(len > 0)
		{
			char last = directories[len - 1];
			if(last != '/' && last != '\\')
				directories.append("/");
		}
	}
}
