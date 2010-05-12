#include "../../corpuslib_headers.h"
#include "SentenceReader.h"

namespace Corpus
{
	namespace ExtSentencer
	{
		CSentenceReader::~CSentenceReader()
		{
		}
		
		void CSentenceReader::Free(Corpus::CSentence *sent)
		{
			delete sent;
		}
		
	} // namespace ExtSentencer
} // namespace Corpus
