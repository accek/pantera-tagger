#include "../../corpuslib_headers.h"
//#include "../GuessingSystem/FormsIdentifier.h"
//#include "Loader.h"
#include "TokenReader.h"

namespace Corpus
{
	namespace Tokenisation
	{
		CTokenReader::CTokenReader(std::istream &co_inStream)
			:co_input(co_inStream)
		{
		}

		CTokenReader::~CTokenReader()
		{
		}
		
		void CTokenReader::Free(Corpus::CToken *token)
		{
			delete token;
		}
		
	} // namespace Tokenisation
} // namespace Corpus

