#include "../../corpuslib_headers.h"
#include "PremorphReader.h"

namespace Corpus
{
	namespace Tokenisation
	{
		CPremorphReader::CPremorphReader(std::istream &co_inStream,
										 std::ostream *co_tagOutStream,
										 bool useMorpher, bool reencode,
										 bool noNewTags)
			:CTokenReader(co_inStream), fragReader(co_inStream),
				tokrUseMorpher(useMorpher), tokrReencode(reencode),
				tokrNoNewTags(noNewTags)
		{
			co_tagoutput = co_tagOutStream;
			fragTokr = NULL;
		}
		
		CPremorphReader::~CPremorphReader()
		{
			delete fragTokr;
		}
		
		CToken *CPremorphReader::GetNext()
		{
			if(fragTokr == NULL)
			{ // used before NextFragment or an unexpected error occurred
				return NULL;
			}
			
			return fragTokr->GetNext();
		}
		
		bool CPremorphReader::NextFragment()
		{
			delete fragTokr;
			fragTokr = NULL; // in case it gets terminated
			fragBuffer.str(""); // clear the buffer
			
			if(!GetPlainFragment())
				return false;
			
			// put fragment into the buffer and create a tokeniser
			fragBuffer.clear();
			fragBuffer << fragReader.frag;
			fragTokr = new CTokeniser(fragBuffer, tokrUseMorpher, tokrReencode,
									  tokrNoNewTags);
			
			return true;
		}
		
		bool CPremorphReader::GetPlainFragment()
		{
			while(true)
			{
				if(!fragReader.NextFragment())
				{ // no fragments left
					return false;
				}
				
				if(fragReader.type == XmlUtils::CFragmentReader::tag)
				{ // flush it directly to tag output
					if(co_tagoutput != NULL)
					{
						(*co_tagoutput)
								<< XmlUtils::CFragmentReader::TAG_OPENING
								<< fragReader.frag
								<< XmlUtils::CFragmentReader::TAG_CLOSING
								<< std::endl;
					}
				}
				else if(!fragReader.frag.empty())
				{ // the fragment is ready in fragReader
					return true;
				}
			}
		}
	}
}
