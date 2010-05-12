#include "../../corpuslib_headers.h"
#include "TokLayer.h"

namespace Corpus
{
	namespace Tokenisation
	{
		CTokLayer::CTokLayer(boost::shared_ptr<CTokLayer> lowerLayer)
			:lower(lowerLayer)
		{
		}

		CTokLayer::CTokLayer()
			:lower()
		{
		}

		CTokLayer::~CTokLayer()
		{
		}

	} // namespace Tokenisation
} // namespace Corpus
