#include "../../../corpuslib_headers.h"
#include "Prefixes.h"


namespace Corpus
{
	namespace GuessingSystem
	{
		namespace BaseEnding
		{
			
			std::string Prefixes::ProcessPrefixes(std::string cp_lowerform)
			{
				for(size_t prefNum = 0; prefNum < prefixes.size(); prefNum++)
				{
					std::string &prefix = prefixes[prefNum];
					if(cp_lowerform.compare(0, prefix.length(), prefix) == 0)
					{
						std::string rev_pref(prefix);
						reverse(rev_pref.begin(), rev_pref.end());
						return cp_lowerform + "#" + rev_pref + "#";
					}
				}
				
				return cp_lowerform;
			}
			
			Prefixes *Prefixes::Deserialize(std::string filename)
			{
				Prefixes *instance = GetInstance();
				std::ifstream in(filename.c_str(), std::ios::binary);
				if(!in)
					return instance;
				
				instance->prefixes.clear();
				std::string cp_line;
				do
				{
					std::getline(in, cp_line);
					boost::algorithm::trim(cp_line);
					if(cp_line.empty())
					{
						break;
					}
					instance->prefixes.push_back(cp_line);
				}
				while(in);
				std::cerr << "Wczytano " << instance->prefixes.size() << " prefiksow\n";
				return instance;
			}
			
			Prefixes *Prefixes::GetInstance()
			{
				static Prefixes instance;
				return &instance;
			}
		}
	}
}
