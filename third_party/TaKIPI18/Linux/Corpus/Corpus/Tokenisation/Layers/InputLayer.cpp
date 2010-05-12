#include "../../../corpuslib_headers.h"
#include "InputLayer.h"
#include "../../Parser/Parser.h"
#include "../../Config.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			CInputLayer::CInputLayer(std::istream &co_inStream)
				:co_input(co_inStream)
			{
			}

			CInputLayer::~CInputLayer()
			{
			}

			boost::shared_ptr<SPretoken> CInputLayer::GetNext()
			{
				CCore *core = GetCore();
				while(pretokens.empty()) // until a non-blank group is found
				{
					std::stringstream co_buffer;
					
					bool wasBlankChar = true, isBlankChar;
					int ch;
					
					while(true)
					{
						ch = co_input.get();
						// end of input
						if(!co_input.good())
							break;
	
						// first, split according to (almost) universal blanks
						isBlankChar = core->IsBlankUniversal(ch);
	
						// a new pretoken
						if(!wasBlankChar && isBlankChar)
							break;
	
						wasBlankChar = isBlankChar;
						if(!isBlankChar)
							co_buffer.put(ch);
					}
					if(co_buffer.rdbuf()->in_avail() == 0) // no chars in token
					{
						boost::shared_ptr<SPretoken> nullPretoken;
						return nullPretoken;
					}
					
					std::string cp_preorth = Parser::Convert(co_buffer.str(),
							CConfig::inputEncoding, Parser::cp1250);
					
					// split it according to cp1250 blanks
					SplitAndPush(cp_preorth);
				}
				
				boost::shared_ptr<SPretoken> returning(pretokens.front());
				pretokens.pop();
				return returning;
			} // GetNext
			
			void CInputLayer::SplitAndPush(const std::string &cp_unsplit)
			{
				// plain space (just in case) or NBSP (cp1250-specific)
				const static std::string cp_delimiters = " \xA0";
				
				// index of first non-delimiter (or std::string::npos)
				size_t lastPos = cp_unsplit.find_first_not_of(cp_delimiters, 0);
				while(lastPos != std::string::npos)
				{
					// index of first delimiter after lastPos (or std::string::npos, a big number)
					size_t pos = cp_unsplit.find_first_of(cp_delimiters, lastPos);
	
					// new group, flush it as a pretoken with NoSpace==false
					boost::shared_ptr<SPretoken> pretoken(
							new SPretoken(cp_unsplit.substr(lastPos, pos - lastPos), false));
					pretokens.push(pretoken);
					
					// skip delimiters (if pos == std::string::npos, it will also give npos)
					lastPos = cp_unsplit.find_first_not_of(cp_delimiters, pos);
				}
			}
			
		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus
