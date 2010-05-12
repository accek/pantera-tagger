#include "../../../corpuslib_headers.h"
#include "FragmentReader.h"
#include "../../entities.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace XmlUtils
		{
			const char CFragmentReader::TAG_OPENING = '<';
			const char CFragmentReader::TAG_CLOSING = '>';
			
			CFragmentReader::CFragmentReader(std::istream &co_inputStream)
				:co_input(co_inputStream)
			{
				state = plain; // no tag so far
				type = plain;
			}
			
			CFragmentReader::~CFragmentReader()
			{
			}
			
			bool CFragmentReader::NextFragment(bool decodeEntities)
			{
				type = state; // remember the type of the fragment
				if(state == plain)
				{
					if(!std::getline(co_input, frag, TAG_OPENING))
						return false; // input terminated
					state = tag;
					if(decodeEntities) DecodeEntities(frag);
					return true;
				}
				else if(state == tag)
				{
					if(!std::getline(co_input, frag, TAG_CLOSING))
						return false; // input terminated
					state = plain;
					return true;
				}
				
				// should never happen
				return false;
			}
			
			void CFragmentReader::Dump()
			{
				std::cout << "FRAGMENT TYPE: ";
				if(type == plain)
					std::cout << "plain";
				else if(type == tag)
					std::cout << "tag";
				else
					std::cout << "UNKNOWN";
				
				std::cout << "\n["
						<< frag << "]\n";
			}
			
		} // namespace XmlUtils
	} // namespace Tokenisation
} // namespace Corpus
