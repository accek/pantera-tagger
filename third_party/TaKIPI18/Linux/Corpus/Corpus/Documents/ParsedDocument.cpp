#include "../../corpuslib_headers.h"
#include "ParsedDocument.h"

namespace Corpus
{
	namespace Documents
	{
		ParsedDocument::ParsedDocument(std::string filename)
			:Document(filename)
		{
		}

		ParsedDocument::~ParsedDocument(void)
		{
		}

		void ParsedDocument::Open() throw(std::string)
		{
			//TODO: Reader �ukasza �arczy�skiego (albo pochodna)
			this->reader = 0;
		}

		CToken* ParsedDocument::NextToken()
		{
			//TODO: Zaprz�c Reader �ukasza (albo pochodn�)
			return 0;
		}

		CSentence* ParsedDocument::NextSentence()
		{
			//TODO: Zaprz�c Reader �ukasza (albo pochodn�)
			return 0;
		}
	}
}
