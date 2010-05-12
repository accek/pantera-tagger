#include "../../corpuslib_headers.h"
#include "Context.h"

namespace Corpus
{
	namespace Context
	{
		Context::Context(Corpus::Documents::Document* doc) :document(doc)
		{
		}

		Context::~Context()
		{
			document->Close();
		}
	}
}
