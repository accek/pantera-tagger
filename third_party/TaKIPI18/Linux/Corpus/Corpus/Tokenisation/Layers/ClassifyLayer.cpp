#include "../../../corpuslib_headers.h"
#include "ClassifyLayer.h"

namespace Corpus
{
	namespace Tokenisation
	{
		namespace Layers
		{
			boost::shared_ptr<SPretoken> CClassifyLayer::GetNext()
			{
				// fetch next
				boost::shared_ptr<SPretoken> current(lower->GetNext());

				// try to classify only uknown pretokens (i.e. of ign class)
				if(current && current->flexClass == cf_ign)
				{
					if(!GetCore()->Classify(*current))
					{
						// if the regexes fail, assume it's a symbol
						current->flexClass = cf_tsym;
						current->attrValues = gc_none;
					}
				}

				return current;
			} // GetNext

		} // namespace Layers
	} // namespace Tokenisation
} // namespace Corpus
